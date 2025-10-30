#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <unistd.h>

typedef struct s_settings t_settings;

typedef struct s_fork
{
    int             id;
    pthread_mutex_t mutex;
}                   t_fork;

typedef struct s_philo
{
    int             id;
    int             status;
    int             meals_eaten;
    unsigned long   last_meal;
    t_fork          *right_hand;
    t_fork          *left_hand;
    pthread_t       thread;
    t_fork          *forks;
    t_settings      *settings;
}                   t_philo;

typedef enum e_status
{
    EATING,
    THINKING,
    SLEEPING,
    DEAD,
    FULL
}   t_status;

typedef struct s_settings
{
    int     num_philo;
    int     time_to_die;
    int     time_to_eat;
    int     time_to_sleep;
    int     num_required_meals;
    int     all_threads_created;
    long    started;
}           t_settings;

typedef struct s_data
{
    t_settings  settings;
    t_philo*    philos;
    int         all_threads_created;
}   t_data;

t_settings set_settings(int num_philo, int time_to_die, int time_to_eat, int time_to_sleep, int num_required_meals)
{
    t_settings set;

    set.num_philo = num_philo;
    set.time_to_die = time_to_die;
    set.time_to_eat = time_to_eat;
    set.time_to_sleep = time_to_sleep;

    set.num_required_meals = num_required_meals;
    set.all_threads_created = 0;
    return (set);
}

t_fork *create_forks(t_settings *settings)
{
    int     i;
    int     res;
    t_fork  *forks;

    forks = malloc(sizeof(t_fork) * settings->num_philo);
    if (!forks)
        return NULL;
    i = 0;
    while (i < settings->num_philo)
    {
        forks[i].id = i;
        res = pthread_mutex_init(&forks[i].mutex, NULL);
        if (res != 0)
            return NULL;
        i++;
    }
    return (forks);
}

int modulo(int x, int N)
{
    return (x % N + N) % N;
}

t_philo *create_philo(t_settings *set, t_fork *forks)
{
    t_philo *philos;
    int i;

    philos = malloc(sizeof(t_philo) * set->num_philo);
    if (!philos)
        return NULL;
    i = 0;
    while(i < set->num_philo)
    {
        philos[i].id = i + 1;
        philos[i].meals_eaten = 0;
        philos[i].status = THINKING;
        philos[i].settings = set;
        philos[i].forks = forks;
        if (philos[i].id % 2 == 0)
        {
            philos[i].left_hand = &forks[philos[i].id - 1];
            philos[i].right_hand = &forks[modulo(philos[i].id - 2, set->num_philo)];
        }
        else
        {
            philos[i].left_hand = &forks[modulo(philos[i].id - 2, set->num_philo)];
            philos[i].right_hand = &forks[philos[i].id - 1];
        }
        i++;
    }
    return (philos);
}

void print_mutex(int philo, char *s)
{
    printf("%d %s\n", philo, s);
}

long get_current_time()
{
    struct timeval time;

    if (gettimeofday(&time, NULL) < 0)
    {
        printf("Error: Unable to use getimeofday");
        return -1;
    }
    return (time.tv_sec * 1000 + time.tv_usec / 1000);
}

void better_usleep(long milis)
{
    long now;

    now = get_current_time();
    while ((get_current_time() - now < milis))
            usleep(500);
}

void eating (t_philo *philo)
{
    pthread_mutex_lock(&philo->left_hand->mutex);
    printf("Philo %d took Left Hand Fork %d\n", philo->id, philo->left_hand->id);
    pthread_mutex_lock(&philo->right_hand->mutex);
    printf("Philo %d took Right Hand Fork %d\n", philo->id, philo->right_hand->id);

    philo->status = EATING;
    philo->meals_eaten++;
    philo->last_meal = get_current_time();
    better_usleep(philo->settings->time_to_eat);
    if (philo->settings->num_required_meals > 0 &&
        philo->meals_eaten == philo->settings->num_required_meals)
    philo->status = FULL;
}

void sleeping(t_philo *philo)
{
    philo->status = SLEEPING;
    better_usleep(philo->settings->time_to_sleep);
}

void thinking(t_philo *philo)
{

}

void *start_philo(void *arg)
{
    t_philo *p;

    p = (t_philo *)arg;
    while (p->settings->all_threads_created != 1);


    //printf("Philo %d holds forks %d and %d\n", p->id, p->right_hand->id, p->left_hand->id);
    //print_mutex(p->id, "started");


    while (1)
    {
        eating(p);
        sleeping(p);
        thinking(p);
    }


    return (arg);
}

int start_simulation(t_settings *settings, t_philo *philos)
{
    int i;
    int res;

    if (settings->num_required_meals == 0)
        return 0;
    if (settings->num_philo == 1)
        return 0;
    i = 0;
    while (i < settings->num_philo)
    {
        res = pthread_create(&philos[i].thread, NULL, start_philo, (void *)&(philos[i]));
        if (res != 0)
            return (-1);
        i++;
    }
    settings->all_threads_created = 1;
    i = 0;
    while(i < settings->num_philo)
    {
        pthread_join(philos[i].thread, NULL);
        i++;
    }
    return (0);
}

int main()
{
    t_settings  settings;
    t_philo     *philos;
    t_fork      *forks;

    settings = set_settings(5, 100, 100, 10, -1);
    forks = create_forks(&settings);
    if (!forks)
    {
        return 1;
    }
    philos = create_philo(&settings, forks);
    if (!philos)
    {
        free(forks);
        return 1;
    }
    if (start_simulation(&settings, philos) < 0)
    {
        return 1;
    }
    return (0);
}
