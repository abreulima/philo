#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

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
    t_fork          right_hand;
    t_fork          left_hand;
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

t_settings set_settings(int num_philo, int time_to_die, int time_to_eat, int num_required_meals)
{
    t_settings set;

    set.num_philo = num_philo;
    set.time_to_die = time_to_die;
    set.time_to_eat = time_to_eat;
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

t_philo *create_philo(t_settings *settings, t_fork *forks)
{
    t_philo *philos;
    int i;

    philos = malloc(sizeof(t_philo) * settings->num_philo);
    if (!philos)
        return NULL;
    i = 0;
    while(i < settings->num_philo)
    {
        philos[i].id = i;
        philos[i].meals_eaten = 0;
        philos[i].status = THINKING;
        philos[i].settings = settings;
        philos[i].forks = forks;
        i++;
    }
    return (philos);
}

void print_mutex(int philo, char *s)
{
    printf("%d %s\n", philo, s);
}

void *start_philo(void *arg)
{
    t_philo *p;
    p = (t_philo *)arg;

    while (p->settings->all_threads_created != 1)
        ;

    print_mutex(p->id, "started");

    return arg;
}

int start_simulation(t_settings *settings, t_philo *philos)
{
    int i;
    int res;

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

    settings = set_settings(5, 100, 10, 0);
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
