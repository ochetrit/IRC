/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   irc.hpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ochetrit <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/18 13:24:39 by ochetrit          #+#    #+#             */
/*   Updated: 2025/02/18 13:24:39 by ochetrit         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef IRC_HPP

#define IRC_HPP

#include <string>
#include <iostream>
#include <cstdlib>
#include <sys/socket.h>
#include <netinet/in.h>
#include <cstring>
#include <arpa/inet.h>
#include <unistd.h>
#include <stdio.h>
#include <map>
#include "clients.hpp"

#define print(x) std::cout << x << std::endl
#define err(x) std::cerr << x << std::endl
#define MAX_PORT 65535
#define FD_MAX 1020
#define MAG "\e[0;35m"
#define RESET   "\033[0m"
#define RED     "\033[31m"
#define GREEN   "\033[32m"
#define YELLOW  "\033[33m"
#define BLUE    "\033[34m"

// irssi USE THIS CLIENT

class IRC
{
	private :

	unsigned int _port;
	std::string _password;

	IRC();

	public :

	IRC(unsigned int port, std::string password);
	IRC(const IRC &other);

	~IRC();

	IRC	&operator=(const IRC &other);

	unsigned int	getPort();

};



#endif