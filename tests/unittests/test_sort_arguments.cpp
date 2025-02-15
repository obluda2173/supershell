#include "test_main.hpp"

struct sortArgumentsParams {
	std::vector<std::string> args;
	std::vector<std::string> want;
};

class sortArgumentsTest : public::testing::TestWithParam<sortArgumentsParams>{};

TEST_P(sortArgumentsTest, allTests) {
	sortArgumentsParams params = GetParam();

	t_list* args = NULL;
	for (size_t i = 0; i < params.args.size(); i++)
		ft_lstadd_back(&args, ft_lstnew(new_argument_pointer(params.args[i].c_str(), LITERAL)));

	sort_arguments(&args);
	ASSERT_EQ(params.want.size(), ft_lstsize(args));
	t_list* head = args;
	for (size_t i = 0; i < params.want.size(); i++ ) {
		ASSERT_EQ(params.want[i], ((t_argument*)head->content)->word);
		head = head->next;
	}
	ft_lstclear(&args, free_arguments);
}

INSTANTIATE_TEST_SUITE_P(sortArgumentsTest, sortArgumentsTest,
						 testing::Values(
							 sortArgumentsParams{{"aaa"}, {"aaa"}},
							 sortArgumentsParams{{"aaa", "bbb"}, {"aaa", "bbb"}},
							 sortArgumentsParams{{"bbb", "aaa"}, {"aaa", "bbb"}},
							 sortArgumentsParams{{"BBB", "aaa"}, {"BBB", "aaa"}},
							 sortArgumentsParams{{"aaa", "BBB"}, {"BBB", "aaa"}},
							 sortArgumentsParams{{"a", "b", "c"}, {"a", "b", "c"}},
							 sortArgumentsParams{{"b", "c", "a"}, {"a", "b", "c"}},
							 sortArgumentsParams{{"c", "b", "a"}, {"a", "b", "c"}},
							 sortArgumentsParams{{"C", "b", "a"}, {"C", "a", "b"}},
							 sortArgumentsParams{{}, {}}
							 )
	);

TEST(sortArgsTestSuite, firstTest) {
	t_list *args = ft_lstnew(new_argument_pointer("aaa", LITERAL));
	sort_arguments(&args);

	t_argument* cur  = (t_argument*)args->content;
	ASSERT_STREQ("aaa", cur->word);

	ft_lstclear(&args, free_arguments);

	args = ft_lstnew(new_argument_pointer("aaa", LITERAL));
	ft_lstadd_back(&args,ft_lstnew(new_argument_pointer("bbb", LITERAL)));
	sort_arguments(&args);

	cur  = (t_argument*)args->content;
	ASSERT_STREQ("aaa", cur->word);

	cur  = (t_argument*)args->next->content;
	ASSERT_STREQ("bbb", cur->word);

	ft_lstclear(&args, free_arguments);
}
