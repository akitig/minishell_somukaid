/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: somukaid <somukaid@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/08 10:10:00 by somukaid          #+#    #+#             */
/*   Updated: 2025/04/23 17:04:02 by somukaid         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

/*
 * DESCRIPTION
 *     ft_echo 関数は、引数として与えられたコマンド文字列（例: "echo hello" や "echo -n hello"）を
 *     解釈して、標準出力に文字列を表示します。
 *
 *     - 入力文字列は "echo " を含む前提で、先頭5文字をスキップします。
 *     - "-n" オプションが存在する場合（例: "echo -n hello"）は改行なしで出力します。
 *     - "-n" オプションがない場合は末尾に改行を付けて出力します。
 */
void	ft_echo(char *node_value)
{
	char	*str;

	str = node_value + 5;
	if (node_value == NULL)
		return ;
	if (!ft_strncmp(str, "-n", 2))
		printf("%s", str + 3);
	else
		printf("%s\n", str);
	return ;
}

/*
 * DESCRIPTION
 *     ft_pwd 関数は、カレントディレクトリの絶対パスを取得し、
 *     それを標準出力に表示します。表示後、取得した文字列のメモリは解放されます。
 *
 *     - getcwd 関数を使用して現在の作業ディレクトリを動的に取得します。
 *     - NULL が返された場合は何も出力せず関数を終了します。
 *     - 正常に取得できた場合はパスを表示し、取得したメモリを解放します。
 */
void	ft_pwd(void)
{
	char	*pwd;

	pwd = getcwd(NULL, 0);
	if (pwd == NULL)
		return ;
	printf("%s\n", pwd);
	free(pwd);
}

/*
 * DESCRIPTION
 *     ft_exit 関数は、与えられた文字列から終了ステータスを取得し、
 *     終了コードを指定してプログラムを終了します。
 *
 *     - 入力文字列が5文字以上の場合（例: "exit 42"）、"exit " の後に続く数字を
 *       整数として解釈し、その値で exit を呼び出します。
 *     - 入力文字列が短い場合は、終了コード 0 でプログラムを終了します。
 *     - 終了前に "exit" を標準出力に表示します。
 */
int	ft_exit(char *str)
{
	int	ret;

	if (ft_strlen(str) > 4)
	{
		printf("exit_number_is_here\n");
		ret = ft_atoi(str + 5);
	}
	else
		ret = 0;
	printf("exit\n");
	exit (ret);
}

/*
 * DESCRIPTION
 *     ft_env 関数は、環境変数のリストを走査し、それぞれの環境変数を
 *     標準出力に表示します。
 *
 *     - 引数として渡された連結リスト envp_list を先頭から末尾まで巡回します。
 *     - 各ノードの content は環境変数の文字列と仮定し、それを printf によって出力します。
 *     - リストが NULL の場合は何もせずに終了します。
 */
void	ft_env(t_list *envp_list)
{
	t_list	*tmp;

	if (envp_list == NULL)
		return ;
	tmp = envp_list;
	while (tmp)
	{
		printf("%s\n", (char *)tmp->content);
		tmp = tmp->next;
	}
}

/*
 * DESCRIPTION
 *     ft_export 関数は、指定されたキーと値の形式の文字列を環境変数リストに追加または更新します。
 *
 *     - key は "NAME=VALUE" 形式の文字列である必要があります。
 *     - 既存の環境変数リスト envp_list に同じキー名が存在する場合は、その値を上書きします。
 *     - 同じキー名が存在しない場合は、新しいノードを作成してリストの末尾に追加します。
 *     - キー名は '=' までの文字列で判定します。'=' が含まれない場合は何も行いません。
 *     - リストや key が NULL の場合は処理を行いません。
 */
void	ft_export(t_list *envp_list, char *key)
{
	t_list	*tmp;
	char	*key_name;
	size_t	len;

	if (envp_list == NULL || key == NULL)
		return ;
	tmp = envp_list;
	len = ft_strchr(key, '=') - key;
	key_name = ft_substr(key, 0, len);
	if (key_name == NULL)
		return ;
	while (tmp)
	{
		if (!ft_strncmp(key_name, (char *)tmp->content, len))
		{
			free(tmp->content);
			tmp->content = (void *)ft_strdup(key);
			free(key_name);
			return ;
		}
		tmp = tmp->next;
	}
	if (tmp == NULL)
	{
		if (ft_strchr(key, '='))
		{
			tmp = ft_lstnew(ft_strdup(key));
			ft_lstadd_back(&envp_list, tmp);
		}
	}
	free(key_name);
}

/*
 * DESCRIPTION
 *     ft_unset 関数は、指定されたキー名に一致する環境変数を環境変数リストから削除します。
 *
 *     - 環境変数リスト env は t_list のポインタへのポインタとして渡されます。
 *     - 各ノードの content は "NAME=VALUE" 形式の文字列であると仮定します。
 *     - 指定された key と一致するキー名（= の前まで）が見つかると、そのノードをリストから削除し、
 *       再帰的に同じキー名を持つ他のノードも削除します。
 *     - リストまたは key が NULL の場合は何も行いません。
 */
void	ft_unset(t_list **env, char *key)
{
	t_list	*tmp;

	if (env == NULL || *env == NULL || key == NULL)
		return ;
	if (!ft_strncmp((char *)(*env)->content, key, ft_strlen(key)) && \
		(size_t)(ft_strchr((char *)(*env)->content, '=') - (char *)(*env)->content) == ft_strlen(key))
	{
		tmp = *env;
		*env = (*env)->next;
		free(tmp->content);
		free(tmp);
		ft_unset(env, key);
	}
	else
		ft_unset(&(*env)->next, key);
}

void	op_command(t_node *node, t_list *envp_list)
{
	if (node->type == 3)
	{
		printf("echo This is redirect.\n");
		op_command(node->left, envp_list);
		if (node->right)
		{
			printf("This is object.\n");
			printf("%s\n", node->right->value);
		}
	}
	if (node->type == 2)
	{
		printf("This is semicolon.\n");
		op_command(node->left, envp_list);
		if (node->right)
			op_command(node->right, envp_list);
	}
	else if (node->type == 1)
	{
		if (!ft_strncmp(node->value, "echo", 4))
			ft_echo(node->value);
		else if (!ft_strncmp(node->value, "pwd", 3))
			ft_pwd();
		else if (!ft_strncmp(node->value, "exit", 4))
			ft_exit(node->value);
		else if (!ft_strncmp(node->value, "env", 3))
			ft_env(envp_list);
		else if (!ft_strncmp(node->value, "export", 6))
			ft_export(envp_list, node->value + 7);
		else if (!ft_strncmp(node->value, "unset", 5))
			ft_unset(&envp_list, node->value + 6);
		else
			printf("NOT_BUILTIN:%s\n", node->value);
	}
	else if (node->type == 0)
	{
		printf("This is Pipe.\n");
		op_command(node->left, envp_list);
		if (node->right)
			op_command(node->right, envp_list);
	}
}