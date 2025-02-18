/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   clients.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ochetrit <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/18 17:27:00 by ochetrit          #+#    #+#             */
/*   Updated: 2025/02/18 17:27:01 by ochetrit         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CLIENTS_HPP
# define CLIENTS_HPP

#include "irc.hpp"

typedef struct s_clients
{
	std::string _nickname;
	std::string	_username;
}	t_clients;

#endif