/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   irc.cpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nclassea <nclassea@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/18 13:36:14 by ochetrit          #+#    #+#             */
/*   Updated: 2025/03/18 17:05:06 by nclassea         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/Irc.hpp"

IRC::IRC() {}

IRC::IRC(unsigned int port, std::string password) : _port(port), _password(password), _nb_clients(0), _servername("ft_irc") {}

IRC::~IRC() {
	std::cout << "The server is closed" << std::endl;
}

IRC::IRC(const IRC &other)
: _port(other._port), _password(other._password) {}

IRC &IRC::operator=(const IRC &other) {
	if (this != &other) {
		_port = other._port;
		_password = other._password;
	}
	return *this;
}

unsigned int IRC::getPort() { return _port; }
unsigned int IRC::getNbclients() { return _nb_clients; }
struct pollfd *IRC::getFds() { return _fds; }
// std::string IRC::getPassword() { return _password; }
std::map<std::string, Channel> &IRC::getChannel() { return _channels; }

Client &IRC::getClient(unsigned int clientFd) {
	return _clients[clientFd];
}

void IRC::add_fds(int fd) {
	_fds[_nb_clients].fd = fd;
	_fds[_nb_clients].events = POLLIN;
	_fds[_nb_clients].revents = 0;
	_nb_clients++;
}

void IRC::set_client_empty(unsigned int clientFd) {
	_clients[clientFd].setNickname("");
	_clients[clientFd].setUsername("");
}

void IRC::set_client_nickname(unsigned int clientFd, std::string &nickname) {
	_clients[clientFd].setNickname(nickname);
}

void IRC::set_client_username(unsigned int clientFd, std::string &username) {
	_clients[clientFd].setUsername(username);
}

void IRC::set_client_pass(unsigned int clientFd, std::string &password) {
	_clients[clientFd].setPass(password);
}

void IRC::set_client_host(unsigned int clientFd, std::string &host) {
	_clients[clientFd].setHost(host);
}

void IRC::decremente_nbclient() { _nb_clients--; }

int IRC::getClientfd(unsigned int client) { return _fds[client].fd; }

bool IRC::compare_nickname(std::string nickname) {
	for (std::map<int, Client>::iterator it = _clients.begin(); it != _clients.end(); ++it) {
		if (it->second.getNickname() == nickname)
		return false;
	}
	return true;
}

bool IRC::compare_username(std::string username) {
	for (std::map<int, Client>::iterator it = _clients.begin(); it != _clients.end(); ++it) {
		if (it->second.getUsername() == username)
		return false;
	}
	return true;
}

void IRC::clearCommands() {
	_commands.clear();
}

void IRC::clearClientBuffers() {
	_client_buffers.clear();
}

void IRC::cleanup() {
	_clients.clear();
	_channels.clear();
	_client_buffers.clear();
	_commands.clear();
}

void IRC::add_channel(const std::string name, unsigned int client) {
	Channel newChannel("", false, false, "", false, FD_MAX);

	// Aadd client and define him as operator
	newChannel.addClient(client);
	newChannel.setOperator(client, true);

	// add chan in _channels
	_channels[name] = newChannel;

	std::string joinMsg = get_prefix(client) + " JOIN :" + name + "\r\n";
	sendAndDisplay(client, joinMsg);

	// sendUserList(client, name);
}


void IRC::add_client_channel(const std::string name, unsigned int client) {
	if (_channels.find(name) == _channels.end()) {
		sendAndDisplay(client, ":" + _servername + " 403 " + name + " :No such channel\r\n");
		return;
	}

	Channel &channel = _channels[name];
	channel.addClient(client);

	print("le chan existe deja");
	std::string joinMsg = get_prefix(client) + " JOIN :" + name + "\r\n";
	sendAndDisplay(client, joinMsg);

	// broadcastToChannel(name, get_prefix(client) + " a rejoint le canal " + name + "\r\n");

	// sendUserList(client, name);
}



























// IRC::IRC(){}

// IRC::IRC(unsigned int port, std::string password):_port(port), _password(password), _nb_clients(0), _servername("ft_irc") {}

// IRC::~IRC(){
// 	print("The server is closed");
// }

// IRC::IRC(const IRC &other):_port(other._port), _password(other._password){}

// IRC	&IRC::operator=(const IRC &other)
// {
// 	if (this != &other)
// 	{
// 		_port = other._port;
// 		_password = other._password;
// 	}
// 	return (*this);
// }

// unsigned int	IRC::getPort(){return _port;}

// unsigned int	IRC::getNbclients(){return _nb_clients;}

// struct pollfd	*IRC::getFds(){return _fds;}

// t_clients		&IRC::getClient(unsigned int client){return _clients[client];}

// std::string IRC::getPassword() {
// 	return this->_password;
// }

// std::string IRC::getNick(unsigned int client) {
// 	return _clients[client]._nickname;
// }

// std::string IRC::getUser(unsigned int client) {
// 	return _clients[client]._username;
// }

// std::string IRC::getHost(unsigned int client) {
// 	return _clients[client]._host;
// }

// Map		&IRC::getChannel(){return _channels;}

// void	IRC::add_fds(int fd)
// {
// 	_fds[_nb_clients].fd = fd;
// 	_fds[_nb_clients].events = POLLIN;
// 	_fds[_nb_clients].revents = 0;
// 	_nb_clients++;
// }

// void	IRC::set_client_empty(unsigned int client)
// {
// 	_clients[client]._nickname.clear();
// 	_clients[client]._username.clear();
// }

// void	IRC::set_client_nickname(unsigned int client, std::string &std){
// 	_clients[client]._nickname = std;
// }

// void	IRC::set_client_username(unsigned int client, std::string &std){
// 	_clients[client]._username = std;
// }

// void	IRC::set_client_pass(unsigned int client, std::string &std) {
// 	_clients[client]._pass = std;
// }

// void	IRC::set_client_host(unsigned int client, std::string &std) {
// 	_clients[client]._host = std;
// }

// void	IRC::decremente_nbclient(){_nb_clients--;}

// int		IRC::getClientfd(unsigned int client){return _fds[client].fd;}

// bool	IRC::compare_nickname(std::string nickname)
// {
// 	for (unsigned int i = 1; i < _nb_clients; i++){
// 		if (_clients[i]._nickname == nickname)
// 			return false;
// 	}
// 	return true;
// }

// bool	IRC::compare_username(std::string username)
// {
// 	for (unsigned int i = 1; i < _nb_clients; i++){
// 		if (_clients[i]._username == username)
// 			return false;
// 	}
// 	return true;
// }

// void	IRC::clearCommands() {
// 	_commands.clear();
// }

// void IRC::clearClientBuffers() {
// 	_client_buffers.clear();
// }