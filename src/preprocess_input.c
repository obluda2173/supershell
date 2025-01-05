/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   preprocess_input.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: erian <erian@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/25 10:33:18 by erian             #+#    #+#             */
/*   Updated: 2025/01/05 17:56:41 by erian            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

char *preprocess_input(char *input) {
    const char *operators[] = {">>", ">>", "<<", "||", "&&", "|", "<", ">"};
    size_t len = strlen(input);
    char *result = malloc(len * 2 + 1); // Allocate extra space for added spaces
    if (!result) return NULL;

    size_t i = 0, j = 0;
    while (input[i]) {
        // Handle numbers preceding `>` or `>>`
        if (isdigit(input[i]) && (input[i + 1] == '>' || (input[i + 1] == '>' && input[i + 2] == '>'))) {
            result[j++] = input[i++]; // Add the number
            result[j++] = input[i++]; // Add the `>`
            if (input[i] == '>') result[j++] = input[i++]; // Add `>>` if present
            if (input[i] && input[i] != ' ') result[j++] = ' '; // Add space after
        }
        // Handle heredoc `<<` without space separation
        else if (input[i] == '<' && input[i + 1] == '<') {
            result[j++] = input[i++];
            result[j++] = input[i++];
            while (input[i] && !isspace(input[i]) && input[i] != '>' && input[i] != '|') {
                result[j++] = input[i++]; // Append heredoc identifier without spaces
            }
            if (input[i] && input[i] != ' ') result[j++] = ' '; // Add space after heredoc
        }
        // Handle quotes
        else if (input[i] == '\'' || input[i] == '\"') {
            char quote = input[i++];
            result[j++] = quote;
            while (input[i] && input[i] != quote) {
                result[j++] = input[i++];
            }
            if (input[i]) result[j++] = input[i++];
        }
        // Handle operators
        else {
            int is_operator = 0;
            for (size_t k = 0; k < sizeof(operators) / sizeof(operators[0]); k++) {
                size_t op_len = strlen(operators[k]);
                if (strncmp(&input[i], operators[k], op_len) == 0) {
                    if (j > 0 && result[j - 1] != ' ') result[j++] = ' '; // Add space before
                    strncpy(&result[j], operators[k], op_len);
                    j += op_len;
                    i += op_len;
                    if (input[i] && input[i] != ' ') result[j++] = ' '; // Add space after
                    is_operator = 1;
                    break;
                }
            }
            if (!is_operator) {
                result[j++] = input[i++];
            }
        }
    }
    result[j] = '\0';

    printf("\nLine: %s\n", result);
    
    return result;
}