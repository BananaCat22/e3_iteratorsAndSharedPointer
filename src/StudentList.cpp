#include "StudentList.h"

std::seed_seq g_seed { 1 };
std::random_device g_rd;
std::mt19937 s_mt(g_seed);
std::normal_distribution<float> g_gradeDist(70.0, 10.0);
std::uniform_int_distribution<size_t> g_gradeNumberDist(
    10, s_gradesPerStudentNumber);

std::shared_ptr<float> generateRandomScores(size_t count)
{
    std::shared_ptr<float> result_p { new float[count],
        std::default_delete<float[]>() };
    for (size_t i = 0; i < count; ++i) {
        result_p.get()[i] = std::max(0.0f, std::min(100.0f, g_gradeDist(s_mt)));
    }
    return result_p;
}

StudentList::~StudentList()
{
    while (nullptr != m_head_p) {
        removeFirstStudent_p();
    }
}

void StudentList::insertStudent_p(std::shared_ptr<Student> student_p)
{
    std::shared_ptr<StudentNode> newNode_p { new StudentNode {} };
    newNode_p->setNextStudentNode_p(m_head_p);
    newNode_p->setStudent_p(student_p);
    m_head_p = newNode_p;
}

void StudentList::removeFirstStudent_p()
{
    if (nullptr != m_head_p) {
        std::shared_ptr<StudentNode> doomedNode_p { m_head_p };
        m_head_p = doomedNode_p->getNextStudentNode_p();
    }
}

std::shared_ptr<StudentList::StudentNode>
StudentList::getFirstStudentNode_p() const
{
    return m_head_p;
}

std::shared_ptr<Student> StudentList::getStudentAtIndex(size_t index) const
{
    std::shared_ptr<Student> result { nullptr };
    std::shared_ptr<const StudentNode> selectedStudentNode_p {
        getFirstStudentNode_p()
    };
    while (nullptr != selectedStudentNode_p && 0 < index) {
        selectedStudentNode_p = selectedStudentNode_p->getNextStudentNode_p();
        --index;
    }
    if (nullptr != selectedStudentNode_p) {
        result = selectedStudentNode_p->getStudent_p();
    }
    return result;
}

void StudentList::insertStudentAtIndex(
    std::shared_ptr<Student> student_p, size_t index)
{
    std::shared_ptr<StudentNode> newNode_p { new StudentNode {} };
    newNode_p->setStudent_p(student_p);

    if (0 == index) {
        newNode_p->setNextStudentNode_p(m_head_p);
        m_head_p = newNode_p;
    } else {
        std::shared_ptr<StudentNode> currentStudentNode_p {
            getFirstStudentNode_p()
        };
        std::shared_ptr<StudentNode> lastGoodNodeStudent_p {
            currentStudentNode_p
        };
        while (nullptr != currentStudentNode_p && 1 < index) {
            lastGoodNodeStudent_p = currentStudentNode_p;
            currentStudentNode_p = currentStudentNode_p->getNextStudentNode_p();
            --index;
        }
        if (nullptr != lastGoodNodeStudent_p) {
            newNode_p->setNextStudentNode_p(
                lastGoodNodeStudent_p->getNextStudentNode_p());
            lastGoodNodeStudent_p->setNextStudentNode_p(newNode_p);
        } else {
            m_head_p = newNode_p;
        }
    }
}

size_t StudentList::getNumberOfStudents() const
{
    size_t result = 0;
    std::shared_ptr<StudentNode> currentStudentNode_p {
        getFirstStudentNode_p()
    };
    while (nullptr != currentStudentNode_p) {
        ++result;
        currentStudentNode_p = currentStudentNode_p->getNextStudentNode_p();
    }
    return result;
}
