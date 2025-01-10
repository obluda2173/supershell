#ifndef TEST_MOCKS_H_
#define TEST_MOCKS_H_

#include <gtest/gtest.h>
# include <gmock/gmock.h>

class SystemWrapper {
public:
	MOCK_METHOD0(mockFork, pid_t(void));
};

extern SystemWrapper* g_systemWrapper;

extern "C" pid_t mock_fork(void);
pid_t ForkFailure(int error);


#endif // TEST_MOCKS_H_
