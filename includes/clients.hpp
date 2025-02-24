/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   clients.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nclassea <nclassea@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/18 17:27:00 by ochetrit          #+#    #+#             */
/*   Updated: 2025/02/24 13:59:09 by nclassea         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CLIENTS_HPP
# define CLIENTS_HPP

#include "irc.hpp"

typedef struct s_clients
{
	std::string _nickname;
	std::string	_username;
	std::string _pass;
}	t_clients;

#endif