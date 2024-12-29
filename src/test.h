/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kfreyer <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/29 13:12/20 by kfreyer           #+#    #+#             */
/*   Updated: 2024/12/29 13:12:20 by kfreyer          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

typedef union NodeData
{
	TextNode		text_node;
	CommandNode		command_node;
	ExpansionNode	expansion_node;
}					t_NodeData;
