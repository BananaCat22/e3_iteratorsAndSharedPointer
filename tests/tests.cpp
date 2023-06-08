#include "StudentList.h"
#include <climits>
#include <gtest/gtest.h>
#include <iomanip>
#include <iostream>
#include <string>

/// This is the number of Student instances to create under test
static constexpr size_t s_studentsNumber { 1000000 };

/// This is the distribution of random numbers used to select Student indexes under test
static std::uniform_int_distribution<size_t> s_studentNumberDist(
    0, s_studentsNumber);

///////////////////////////////////////////////////////////////////////////////
TEST(iteratorsAndSharedPointer, construction)
{
    StudentList studentList {};

    GTEST_ASSERT_EQ(0, studentList.getNumberOfStudents());
}

///////////////////////////////////////////////////////////////////////////////
TEST(iteratorsAndSharedPointer, insertAtIndex0)
{
    StudentList studentList {};
    std::shared_ptr<Student> newStudent0_p { new Student { nullptr, 0 } };
    studentList.insertStudentAtIndex(newStudent0_p, 0);

    GTEST_ASSERT_EQ(1, studentList.getNumberOfStudents());

    std::shared_ptr<float> scores_p { new float[3],
        std::default_delete<float[]>() };
    std::shared_ptr<Student> newStudent1_p { new Student { scores_p, 3 } };
    studentList.insertStudentAtIndex(newStudent1_p, 0);

    GTEST_ASSERT_EQ(2, studentList.getNumberOfStudents());
    GTEST_ASSERT_EQ(3, studentList.getStudentAtIndex(0)->getScoresNumber());
    GTEST_ASSERT_EQ(0, studentList.getStudentAtIndex(1)->getScoresNumber());

    studentList.removeFirstStudent_p();
    GTEST_ASSERT_EQ(1, studentList.getNumberOfStudents());
    GTEST_ASSERT_EQ(0, studentList.getStudentAtIndex(0)->getScoresNumber());
}

///////////////////////////////////////////////////////////////////////////////
TEST(iteratorsAndSharedPointer, insertAtIndex1)
{
    StudentList studentList {};
    std::shared_ptr<Student> newStudent_p {};

    for (int32_t i = 10 - 1; i >= 0; --i) {
        const size_t gradesNumber { 5 };
        newStudent_p = std::shared_ptr<Student> { new Student {
            generateRandomScores(gradesNumber), gradesNumber } };
        studentList.insertStudentAtIndex(newStudent_p, i);
    }
    GTEST_ASSERT_EQ(10, studentList.getNumberOfStudents());

    // test for(:) use of iterator
    for (auto student_p : studentList) {
        std::cout << "Student:\n";
        const size_t scoresNumber { student_p->getScoresNumber() };
        for (size_t i = 0; i < scoresNumber; ++i) {
            std::cout << "\t" << student_p->getScores_p().get()[i] << "\n";
        }
    }

    // test for with explicit iterator pre-increment
    for (auto it = studentList.begin(); it != studentList.end(); ++it) {
        std::cout << "Student:\n";
        const size_t scoresNumber { it->getScoresNumber() };
        for (size_t i = 0; i < scoresNumber; ++i) {
            std::cout << "\t" << it->getScores_p().get()[i] << "\n";
        }
    }

    // test for with explicit iterator post-increment
    for (auto it = studentList.begin(); it != studentList.end(); it++) {
        std::cout << "Student:\n";
        const size_t scoresNumber { it->getScoresNumber() };
        for (size_t i = 0; i < scoresNumber; ++i) {
            std::cout << "\t" << it->getScores_p().get()[i] << "\n";
        }
    }

    // test find when element is found
    auto it0 { std::find(
        studentList.begin(), studentList.end(), newStudent_p) };
    GTEST_ASSERT_EQ(*it0, newStudent_p);

    // test find when element is not found
    auto it1 { std::find(
        studentList.begin(), studentList.end(), std::shared_ptr<Student> {}) };
    GTEST_ASSERT_EQ(studentList.end(), it1);
}

///////////////////////////////////////////////////////////////////////////////
TEST(iteratorsAndSharedPointer, leaks)
{
    StudentList studentList {};

    for (int32_t i = 0; i < s_studentsNumber; ++i) {
        const size_t gradesNumber { g_gradeNumberDist(s_mt) };
        std::shared_ptr<Student> newStudent_p { new Student {
            generateRandomScores(gradesNumber), gradesNumber } };
        studentList.insertStudent_p(newStudent_p);
    }
    GTEST_ASSERT_EQ(s_studentsNumber, studentList.getNumberOfStudents());

    const size_t studentIndex { s_studentNumberDist(s_mt) };
    std::cout << "Student index: " << studentIndex << "\n";

    std::shared_ptr<const Student> student_p { studentList.getStudentAtIndex(
        studentIndex) };
    const size_t numberOfScores { student_p->getScoresNumber() };
    std::shared_ptr<const float> scores_p { student_p->getScores_p() };
    std::cout << "Scores:\n" << std::fixed << std::setprecision(1);
    for (size_t i = numberOfScores - 1; i != 0; --i) {
        std::cout << "\t" << scores_p.get()[i] << "\n";
    }
}
TEST(iteratorsAndSharedPOinter, meansScore0)
{
    StudentList studentList {};
    std::shared_ptr<Student> newStudent_p {};

    for (int32_t i = 0; i < 5; ++i) {
        const size_t gradesNumber { 3 };
        std::shared_ptr<float> f_p { new float[3] { 100, 50, 30 },
            std::default_delete<float[]>() };
        newStudent_p
            = std::shared_ptr<Student> { new Student { f_p, gradesNumber } };
        studentList.insertStudent_p(newStudent_p);
    }
    for (auto it = studentList.begin(); it != studentList.end(); ++it) {
        auto meanScore = it->meanScore();
        std::cout << meanScore << "\n";
        GTEST_ASSERT_EQ(true, 0.00001 > (fabs(60.0f - meanScore)));
    }
}

///////////////////////////////////////////////////////////////////////////////
int main(int argc, char* argv[])
{
    ::testing::InitGoogleTest(&argc, argv);
    auto result = RUN_ALL_TESTS();

    std::cout << ((0 != result) ? "\nOne or more tests failed.\n" : "\n");

    exit(0);
    return 0;
}
