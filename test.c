#include "codexion.h"
#include<unistd.h>

pthread_mutex_t	m1;
pthread_cond_t	c1;
int				age = 1;

void	*grow(void *args)
{
	pthread_mutex_lock(&m1);
	printf("Aging...\n");
	age++;
	pthread_cond_signal(&c1);
	pthread_mutex_unlock(&m1);
}

void	*check_age(void *args)
{
	pthread_mutex_lock(&m1);
	while (age < 18)
	{
		printf("Still minor...\n");
		pthread_cond_wait(&c1, &m1);
	}
	printf("You are an adult now!\n");
	pthread_mutex_unlock(&m1);
}

int	main(void)
{
	pthread_t	t1, t2;

	pthread_mutex_init(&m1, NULL);
	pthread_cond_init(&c1, NULL);
	pthread_create(&t2, NULL, check_age, NULL);
	while (age < 19)
	{
		usleep(10);
		pthread_create(&t1, NULL, grow, NULL);
		pthread_join(t1, NULL);
	}
	pthread_join(t2, NULL);
	pthread_mutex_destroy(&m1);
	pthread_cond_destroy(&c1);
}
