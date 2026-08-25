/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   simulation.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbahri <mbahri@student.1337.ma>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/25 22:01:13 by mbahri            #+#    #+#             */
/*   Updated: 2026/08/25 23:13:43 by mbahri           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

int init_dongles(t_simulation *sim);

int init_simulation(t_simulation *sim, t_config *config)
{   
    sim->config = *config;
    sim->stop = 0;
    sim->start_time = 0;
    if (pthread_mutex_init(&sim->state_mutex, NULL) != 0)
        return (0);
    
    if (pthread_mutex_init(&sim->log_mutex, NULL) != 0)
    {
        pthread_mutex_destroy(&sim->state_mutex);
        return (0);
    }

    if (!init_dongles(sim))
        return (0);
}
