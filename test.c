/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbahri <mbahri@student.1337.ma>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/18 23:30:11 by username          #+#    #+#             */
/*   Updated: 2026/04/19 22:14:42 by mbahri           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include <pthread.h>

// int	main(void)
// {
// 	//1- create 10 threads
// 	//2- they all share the same var int share = 0
// 	//3- handle race condition by letting only once thread to incremenat shared at a time
// 	//4- print how threads increment shared at a time, before: shared = 0, after: shared = 1
// 	//shared go from 0 to 10 seamlessly!
// 	//test with mutex and without mutex
// }

// //race condition, mutex, ceating threads, joining threads
// //1- create threads
// //2- create a function that will be passed to the thread
// //3- create join function
// //=====
// // function of the thread had to print:
// // before: shared = 0
// // after: shared = 1
// // and so on till shared = 10
// // learn about race condition
// // learn about how mutex handle race condition bug
// // test your code with and without

void	*task(void *arg)
{
	int	*x;

	x = (int *)(arg);
	x++;
	printf("Hello world!");
	return (NULL);
}

int	main(void)
{
	pthread_t	thread;

	pthread_create(&thread, NULL, task, NULL);
	pthread_join(thread, NULL);
}
