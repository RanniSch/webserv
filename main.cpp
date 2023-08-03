/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rschlott <rschlott@student.42wolfsburg.de> +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/03 19:53:52 by lkavalia          #+#    #+#             */
/*   Updated: 2023/08/03 21:38:48 by rschlott         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "webserv.hpp"

int main(int argc, char **argv)
{
	if (argc != 2)
	{
		std::cout << "Error: incorrect ammount of arguments!" << std::endl;
		return(-1);
	}
	(void)argv;
	
	return (0);
}