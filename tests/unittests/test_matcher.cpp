#include "test_main.hpp"
#include "gtest/gtest.h"
#include <gtest/gtest.h>

struct TestPatternMatchParams {
	const char* pattern;
	const char* str;
	bool want;
};

class TestPatternMatcherSuite : public ::testing::TestWithParam<TestPatternMatchParams> {};

TEST_P(TestPatternMatcherSuite, MatchAll) {
	TestPatternMatchParams params = GetParam();
	const char* pattern = params.pattern;
	const char* str = params.str;
	bool got = matches_pattern(pattern, str);
	ASSERT_EQ(params.want, got);
};


INSTANTIATE_TEST_SUITE_P(
	MatchAll,
	TestPatternMatcherSuite,
	testing::Values(
		TestPatternMatchParams{"*", "", true},
		TestPatternMatchParams{"*", "adsf alles", true}
		)
	);

INSTANTIATE_TEST_SUITE_P(
	MatchBeginning,
	TestPatternMatcherSuite,
	testing::Values(
		TestPatternMatchParams{"start*", "start", true},
		TestPatternMatchParams{"start*", "start alles", true},
		TestPatternMatchParams{"start*", "star", false},
		TestPatternMatchParams{"start*", "", false}
		)
	);

INSTANTIATE_TEST_SUITE_P(
	MatchEnding,
	TestPatternMatcherSuite,
	testing::Values(
		TestPatternMatchParams{"*end", "end", true},
		TestPatternMatchParams{"*end", "nd", false},
		TestPatternMatchParams{"*end", "alles end", true},
		TestPatternMatchParams{"*end", "allesend", true},
		TestPatternMatchParams{"*end", "", false}
		)
	);

INSTANTIATE_TEST_SUITE_P(
	InBetween,
	TestPatternMatcherSuite,
	testing::Values(
		TestPatternMatchParams{"start*end", "startend", true},
		TestPatternMatchParams{"start*end", "start   end", true},
		TestPatternMatchParams{"start*end", "start _\'\"++-0ß end", true},
		TestPatternMatchParams{"start*end", "startnd", false}
		)
	);

INSTANTIATE_TEST_SUITE_P(
	beginning_and_ending,
	TestPatternMatcherSuite,
	testing::Values(
		TestPatternMatchParams{"*some*", "  adsf some  adsfasd", true},
		TestPatternMatchParams{"*some*", "some", true},
		TestPatternMatchParams{"*some*", "some  adsfasd", true},
		TestPatternMatchParams{"*some*", "  adsf some", true},
		TestPatternMatchParams{"*some*", "somadsfasd", false},
		TestPatternMatchParams{"*some*", "asdfome", false}
		)
	);

INSTANTIATE_TEST_SUITE_P(
	multiple_and_ending,
	TestPatternMatcherSuite,
	testing::Values(
		TestPatternMatchParams{"some0*some1*some2*some3", "some0some1some2some3", true},
		TestPatternMatchParams{"some0*some1*some2*some3", "asome0some1some2some3", false},
		TestPatternMatchParams{"some0*some1*some2*some3", "some0some1some2some3a", false},
		TestPatternMatchParams{"some0*some1*some2*some3", "some0some1some2asome3", true},
		TestPatternMatchParams{"some0*some1*some2*some3", "some0some1asome2some3", true},
		TestPatternMatchParams{"some0*some1*some2*some3", "some0asome1asome2some3", true}
		)
	);
