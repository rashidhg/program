#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/time.h>
#include <signal.h>
#include <ucontext.h>

#define N 3
#define SZ 10000

struct thread
{
    int id;
    ucontext_t ctx;
};
typedef struct thread thread_t;

struct linklist
{
    thread_t thr;
    struct linklist* next;
};
typedef struct linklist ll;

int thread_count = 0;

ucontext_t main_context;
ucontext_t scheduler_context;
ucontext_t killer_context;

struct sigaction act;
struct itimerval it;

ll* head;
thread_t* cur_thread;

void block_signal(sigset_t *mask)
{
    sigfillset(mask);
    sigprocmask(SIG_BLOCK, mask, 0);
}

void unblock_signal(sigset_t *mask)
{
    sigfillset(mask);
    sigprocmask(SIG_UNBLOCK, mask, 0);
}

void thread_run()
{
    int i;
    ll* temp;
    while(thread_count != 0)
    {
        temp = head;
        while(temp != NULL)
        {
            cur_thread = &temp->thr;
            swapcontext(&scheduler_context, &cur_thread->ctx);  
            temp = temp->next;
        }
    }
}

void thread_kill()
{
    sigset_t mask;
    ll* temp;
    ll* prev;

    while(thread_count != 0)
    {
        prev = temp = head;
        while(temp != NULL)
        {
        if(cur_thread == &temp->thr)
        {
            block_signal(&mask);
            if(temp == head)
                head = head->next;
            else
                prev->next = temp->next;

            thread_count--;
            if(thread_count == 0)
            exit(1);
            printf("Thread %d exiting\n", cur_thread->id);
            unblock_signal(&mask);
        }
        prev = temp;
        temp = temp->next;
        }
    }
}

void sighandler()
{
    printf("Signal generated, context switching for thread %d\n", cur_thread->id);
    swapcontext(&cur_thread->ctx, &scheduler_context);
}

void thr_function(void* arg)
{
    int id = *(int *)arg;
    int i;
    for (i = id*10; i <= id*10 + 5; ++i)
    {
        printf("%d\n", i);
        sleep(rand() % 2);
    }
}

void init()
{
    head = NULL;

    act.sa_handler = sighandler;
    sigemptyset(&act.sa_mask);
    act.sa_flags = 0;
    sigaction(SIGALRM, &act, 0); 

    getcontext(&scheduler_context);
    scheduler_context.uc_stack.ss_sp = malloc(SZ);
    scheduler_context.uc_stack.ss_size = SZ;
    scheduler_context.uc_stack.ss_flags = 0;
    scheduler_context.uc_link = &main_context;
    makecontext(&scheduler_context, thread_run, 0);

    getcontext(&killer_context);
    killer_context.uc_stack.ss_sp = malloc(SZ);
    killer_context.uc_stack.ss_size = SZ;
    killer_context.uc_stack.ss_flags = 0;
    killer_context.uc_link = 0;
    makecontext(&killer_context, thread_kill, 0);
}

void thread_create(thread_t *t, void (*fp)(), void* arg)
{
    getcontext(&t->ctx);
    t->ctx.uc_stack.ss_sp = malloc(SZ);
    t->ctx.uc_stack.ss_size = SZ;
    t->ctx.uc_stack.ss_flags = 0;
    t->ctx.uc_link = &killer_context;

    if(arg == NULL)
        makecontext(&t->ctx, fp, 0);
    else
        makecontext(&t->ctx, fp, 1, arg);

    if(thread_count == 0)                     //  First thread will start Alarm
    {
        it.it_interval.tv_sec = 1;
        it.it_interval.tv_usec = 10000;
        it.it_value.tv_sec = 1;
        it.it_value.tv_usec = 10000;
        setitimer(ITIMER_REAL, &it, NULL);
    }
    thread_count++;
    /*
    sigset_t mask;
    block_signal(&mask);
    insert(t);
    unblock_signal(&mask);
    */
}

void thread_join(thread_t *t)
{
    swapcontext(&main_context, &scheduler_context);
}

int main()
{
    init();

    head = malloc(sizeof(ll));
    head->thr.id = 1;
    ll* temp = head;

    int i;
    for (i = 1; i < N; ++i)
    {
        temp->next = malloc(sizeof(ll));
        temp = temp->next;
        temp->thr.id = i + 1;
    }

    temp = head;
    while(temp != NULL)
    {
        thread_create(&temp->thr, thr_function, &temp->thr.id);
        temp = temp->next;
    }

    temp = head;
    while(temp != NULL)
    {
        thread_join(&temp->thr);
        temp = temp->next;
    }

    return 0;
}
