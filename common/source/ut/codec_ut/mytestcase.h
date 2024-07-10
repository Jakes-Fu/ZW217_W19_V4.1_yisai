// MyTestCase.h: interface for the CMyTestCase class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MYTESTCASE_H__75923027_4C7A_4627_826B_47DBE529B603__INCLUDED_)
#define AFX_MYTESTCASE_H__75923027_4C7A_4627_826B_47DBE529B603__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <cppunit\TestCase.h>
#include <cppunit\extensions\HelperMacros.h>

class MyTestCase : public CppUnit::TestCase
{
	CPPUNIT_TEST_SUITE(MyTestCase);
	CPPUNIT_TEST(TestCaseGbToUcs2);
	CPPUNIT_TEST(TestCase7bitsTo8bits);
	CPPUNIT_TEST(TestCaseHexStrToBin);
	CPPUNIT_TEST(TestCaseBCDToBin);
	CPPUNIT_TEST_SUITE_END();
public:
	MyTestCase();
	virtual ~MyTestCase();

protected:
	void TestCaseGbToUcs2(void);
	void TestCase7bitsTo8bits(void);
	void TestCaseHexStrToBin(void);
	void TestCaseBCDToBin(void);
};

#endif // !defined(AFX_MYTESTCASE_H__75923027_4C7A_4627_826B_47DBE529B603__INCLUDED_)
