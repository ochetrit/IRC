/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   irc.cpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nclassea <nclassea@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/18 13:36:14 by ochetrit          #+#    #+#             */
/*   Updated: 2025/02/25 16:12:48 by nclassea         ###   ########.fr       */
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

t_clients		IRC::getClient(unsigned int client){return _clients[client];}

std::string IRC::getPassword() {
	return this->_password;
}

Map		IRC::getChannel(){return channels;}

void	IRC::add_fds(int fd)
{
	_fds[_nb_clients].fd = fd;
	_fds[_nb_clients].events = POLLIN;
	_fds[_nb_clients].revents = 0;
	_nb_clients++;
}

void	IRC::set_client_empty(unsigned int client)
{
	_clients[client]._nickname.clear();
	_clients[client]._username.clear();
}

void	IRC::set_client_nickname(unsigned int client, std::string &std){
	_clients[client]._nickname = std;
}

void	IRC::set_client_username(unsigned int client, std::string &std){
	_clients[client]._username = std;
}

void	IRC::set_client_pass(unsigned int client, std::string &std) {
	_clients[client]._pass = std;
}

void	IRC::decremente_nbclient(){_nb_clients--;}

int		IRC::getClientfd(unsigned int client){return _fds[client].fd;}

bool	IRC::compare_nickname(std::string nickname)
{
	for (unsigned int i = 1; i < _nb_clients; i++){
		if (_clients[i]._nickname == nickname)
			return false;
	}
	return true;
}

bool	IRC::compare_username(std::string username)
{
	for (unsigned int i = 1; i < _nb_clients; i++){
		if (_clients[i]._username == username)
			return false;
	}
	return true;
}