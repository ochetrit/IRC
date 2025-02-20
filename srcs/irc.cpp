/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   irc.cpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ochetrit <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/18 13:36:14 by ochetrit          #+#    #+#             */
/*   Updated: 2025/02/18 13:36:14 by ochetrit         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/irc.hpp"

IRC::IRC(){}

IRC::IRC(unsigned int port, std::string password):_port(port), _password(password), _nb_clients(0){}

IRC::~IRC(){
	print("The server is closed");
}

IRC::IRC(const IRC &other):_port(other._port), _password(other._password){}

IRC	&IRC::operator=(const IRC &other)
{
	if (this != &other)
	{
		_port = other._port;
		_password = other._password;
	}
	return (*this);
}

unsigned int	IRC::getPort(){return _port;}

unsigned int	IRC::getNbclients(){return _nb_clients;}

struct pollfd	*IRC::getFds(){return _fds;}

void	IRC::add_fds(int fd)
{
	_fds[_nb_clients].fd = fd;
	_fds[_nb_clients].events = POLLIN;
	_nb_clients++;
}

void	IRC::set_client_empty(int client)
{
	_clients[client]._nickname.empty();
	_clients[client]._username.empty();
}