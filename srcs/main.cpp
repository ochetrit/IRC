/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ochetrit <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/18 13:23:06 by ochetrit          #+#    #+#             */
/*   Updated: 2025/02/18 13:23:06 by ochetrit         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/irc.hpp"


bool	check_args(char *av)
{
	std::string port(av);

	if (port.find_first_not_of("0123456789") != std::string::npos)
	{
		err("I only need numbers for my port");
		return false;
	}

	else if (port.size() > 5)
	{
		err("Port has to be < " << MAX_PORT);
		return false;
	}

	return true;
}


int	main(int ac, char **av)
{
	if (ac != 3)
	{
		err("I need 2 arguments in the format: <port> <password>");
		return 1;
	}

	if (!check_args(av[1]))
		return 2;

	IRC	irc(atoi(av[1]), av[2]);

	int server_fd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

	if (server_fd == -1)
	{
		perror("Socket failed");
		return 3;
	}

	struct sockaddr_in server_addr;
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
	inet_pton(AF_INET, "127.0.0.1", &server_addr.sin_addr);
    server_addr.sin_port = htons(irc.getPort());

	if (bind(server_fd, (struct sockaddr*)&server_addr, sizeof(server_addr)) < 0)
	{
		perror("Error bind");
		close(server_fd);
		return 4;
	}

	print(YELLOW << "Waiting for clients...");

	if (listen(server_fd, FD_MAX) < 0)
	{
		perror("Error listen");
		close(server_fd);
		return 1;
	}

	std::map<int, t_clients> clients;

	while (1)
	{
		struct sockaddr_in client_addr;
		socklen_t client_len = sizeof(client_addr);
		int client_fd = accept(server_fd, (struct sockaddr*)&client_addr, &client_len);
		if (client_fd < 0)
		{
			perror("Error accept");
			close(server_fd);
			return 1;
		}
		print(GREEN << "New client accepted");

		std::string str = "Set a nickname\n";
		send(client_fd, str.c_str(), str.size(), 0);

		t_clients	tmp;

		char buffer[100];
		memset(buffer, 0, sizeof(buffer));
		int bytes_received = recv(client_fd, buffer, sizeof(buffer) - 1, 0);
        if (bytes_received <= 0)
		{
            print(RED << "Client déconnecté" << RESET);
            break;
        }
		tmp._nickname = buffer;

		str = "Set a username\n";
		send(client_fd, str.c_str(), str.size(), 0);

		memset(buffer, 0, sizeof(buffer));
		bytes_received = recv(client_fd, buffer, sizeof(buffer) - 1, 0);
        if (bytes_received <= 0)
		{
            print(RED << "Client déconnecté" << RESET);
            break;
        }
		tmp._username = buffer;
		clients.insert(std::make_pair(client_fd, tmp));
	}
	//close(client_fd);
	close(server_fd);

	return 0;
}
