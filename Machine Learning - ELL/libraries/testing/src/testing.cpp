/**
 * Microsoft - Modern Information Technology
 * https://github.com/Microsoft/ELL/blob/master/libraries/testing/src/testing.cpp
 *
 *  Created on: Dec 26, 2018
 *  Student (MIG Virtual Developer): Tung Dang
 */

#include "testing.h"
#include <utilities/include/Logger.h>

#include <iostream>
#include <string>

namespace ell
{
    namespace testing
    {
        TestFailureException::TestFailureException(const std::string& testDescription) : 
            std::runtime_error(std::string("TestFailureException: ") + testDescription)
        {}

        TestNotImplementedException::TestNotImplementedException(const std::string& testDescription) : 
            std::runtime_error(std::string("TestNotImplementedException: ") + testDescription)
        {
        }

        template <typename ValueType>
        bool IsVectorEqual(const std::vector<ValueType>& a, const std::vector<ValueType>& b)
        {
            auto size = a.size();
            if (size != b.size())
            {
                return false;
            }

            for (size_t index = 0; index < size; ++index)
            {
                if (a[index] != b[index])
                {
                    return false;
                }
            }
            return true;
        }

        template <typename ValueType1, typename ValueType2, typename ValueType3>
        bool IsVectorApproxEqual(const std::vector<ValueType1>& a, const std::vector<ValueType2>& b, ValueType3 tolerance)
        {
            size_t size = a.size();
            if (b.size() < size)
            {
                size = b.size();
            }

            for (size_t i = 0; i < size; ++i)
            {
                if (IsEqual(a[i], b[i], tolerance) == false)
                {
                    return false;
                }
            }

            for (size_t i = size; i < a.size(); ++i)
            {
                if (IsEqual(a[i], static_cast<ValueType1>(0), tolerance) == false)
                {
                    return false;
                }
            }

            for (size_t i = size; i < b.size(); ++i)
            {
                if (IsEqual(b[i], static_cast<ValueType2>(0), tolerance) == false)
                {
                    return false;
                }
            }

            return true;
        }

        bool IsEqual(const std::vector<bool>& a, const std::vector<bool>& b) 
        {
            return IsVectorEqual(a, b);
        }

        bool IsEqual(const std::vector<int>& a, const std::vector<int>& b)
        {
            return IsVectorEqual(a,b);
        }

        bool IsEqual(const std::vector<int64_t>& a, const std::vector<int64_t>& b)
        {
            return IsVectorEqual(a,b);
        }

        bool IsEqual(const std::vector<std::string>& a, const std::vector<std::string>& b)
        {
            return IsVectorEqual(a,b);
        }

        bool IsEqual(const std::vector<float>& a, const std::vector<float>& b, float tolerance)
        {
            return IsVectorApproxEqual(a, b, tolerance);
        }

        bool IsEqual(const std::vector<double>& a, const std::vector<double>& b, double tolerance)
        {
            return IsVectorApproxEqual(a, b, tolerance);
        }

        bool IsEqual(const std::vector<std::vector<float>>& a, const std::vector<std::vector<float>>& b, float tolerance)
        {
            for (size_t index = 0; index < a.size(); ++index)
            {
                if (!IsVectorApproxEqual(a[index], b[index], tolerance))
                {
                    return false;
                }
            }
            return true;
        }

        template <typename ValueType1, typename ValueType2>
        bool IsEqual(const std::vector<std::vector<ValueType1>>& a, const std::vector<std::vector<ValueType2>>& b, double tolerance)
        {
            if (a.size() != b.size())
            {
                return false;
            }
            
            for (size_t index = 0; index < a.size(); ++index)
            {
                if (!IsVectorApproxEqual(a[index], b[index], tolerance))
                {
                    return false;
                }
            }
            return true;
        }

        bool testFailedFlag = false;
        bool ProcessTest(const std::string& testDescription, bool success)
        {
            if (!success)
            {
                TestFailed(testDescription);
            }
            else
            {
                TestSucceeded(testDescription);
            }
            return success;
        }

        bool ProcessQuietTest(const std::string& testDescription, bool success)
        {
            if (!success)
            {
                TestFailed(testDescription);
            }
            return success;
        }

        void ProcessCriticalTest(const std::string& testDescription, bool success)
        {
            if (!ProcessTest(testDescription, success))
            {
                throw TestFailureException(testDescription);
            }
        }

        void TestFailed(const std::string& message)
        {
            std::cout << message << "... Failed" << std::endl;
        }

        void TestSucceeded(const std::string& message)
        {
            std::cout << message << "... Success" << std::endl;
        }

        bool DidTestFail()
        {
            return testFailedFlag;
        }

        int GetExitCode()
        {
            return DidTestFail() ? 1 : 0;
        }

        EnableLoggingHelper::EnableLoggingHelper()
        {
            logging::ShouldLog() = true;
        }

        EnableLoggingHelper::~EnableLoggingHelper()
        {
            logging::ShouldLog() = false;
        }

        template bool IsEqual(const std::vector<std::vector<float>>& a, const std::vector<std::vector<float>>& b, double tolerance);
        template bool IsEqual(const std::vector<std::vector<float>>& a, const std::vector<std::vector<double>>& b, double tolerance);
        template bool IsEqual(const std::vector<std::vector<double>>& a, const std::vector<std::vector<float>>& b, double tolerance);
        template bool IsEqual(const std::vector<std::vector<double>>& a, const std::vector<std::vector<double>>& b, double tolerance);
    }    

} 
