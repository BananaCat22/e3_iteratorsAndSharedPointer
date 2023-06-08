#ifndef StudentList_H
#define StudentList_H

#include <cstddef>
#include <cstdint>
#include <memory>
#include <random>

static constexpr size_t s_gradesPerStudentNumber { 20 };
extern std::seed_seq g_seed;
extern std::random_device g_rd;
extern std::mt19937 s_mt;
extern std::normal_distribution<float> g_gradeDist;
extern std::uniform_int_distribution<size_t> g_gradeNumberDist;

/// This function dynamically allocates floating point "scores".
///\param count : the number of random floats scores to allocate
///\return a pointer to a dynamically allocated array of count random float with
/// g_gradeDist distribution
std::shared_ptr<float> generateRandomScores(size_t count);

/// Each instance of this class encapsulates information about a "student" including
/// some number of scores.
class Student {
    /// The number of scores encapsulated
    size_t m_scoresNumber { 0 };

    /// A dynamically allocated array of m_scoresNumber scores
    std::shared_ptr<const float> m_scores_p { nullptr };

public:
    /// Constructor
    ///\param scores_p : pointer to count scores
    ///\param count : the number of scores in memory starting at scores_p
    Student(std::shared_ptr<const float> scores_p, size_t count)
    {
        m_scores_p = scores_p;
        m_scoresNumber = count;
    }

    /// Destructor
    ~Student() { }

    ///\return the number of encapsulated scores
    size_t getScoresNumber() const { return m_scoresNumber; }

    ///\return pointer to the first of the encapsulated scores
    std::shared_ptr<const float> getScores_p() const { return m_scores_p; }
    /// @return mean score of StudentScore
    float meanScore() const
    {
        float averageScore = 0;
        auto p = getScores_p().get();
        size_t count = getScoresNumber();
        auto end_p = p + count;
        for (; p != end_p; p++) {
            averageScore += *p;
        }
        averageScore = averageScore / count;
        return averageScore;
    }
};

/// Each instance of this class encapsulates a list of Student instances
class StudentList {

    /// Each instance of this class encapsulates a node in the list of students
    class StudentNode {
        friend class StudentList;

        // Pointer to the next node in the list
        std::shared_ptr<StudentNode> m_next_p { nullptr };

        // Pointer to the Student instance associated with the node
        std::shared_ptr<Student> m_student_p { nullptr };

        void setStudent_p(std::shared_ptr<Student> student_p)
        {
            m_student_p = student_p;
        }
        std::shared_ptr<StudentNode> getNextStudentNode_p() const
        {
            return m_next_p;
        }
        void setNextStudentNode_p(std::shared_ptr<StudentNode> studentNode_p)
        {
            m_next_p = studentNode_p;
        }

    public:
        ///\returnpointer to teh Student associated with the node
        std::shared_ptr<Student> getStudent_p() const { return m_student_p; }
    };

    /// Pointer to the first node in the list
    std::shared_ptr<StudentNode> m_head_p { nullptr };

    std::shared_ptr<StudentNode> getFirstStudentNode_p() const;

public:
    /// This type encapsulates the current node pointer for forward
    /// iteration through the list. By implementing an iterator, the
    /// for(:) syntax may be used with the list along with other standard
    /// algorithms like find() and copy().
    struct iterator {
        using iterator_category = std::forward_iterator_tag;
        using difference_type = std::ptrdiff_t;
        using value_type = std::shared_ptr<Student>;
        using pointer = value_type*; // or also value_type*
        using reference = value_type&; // or also value_type&

        iterator(std::shared_ptr<StudentNode> ptr = nullptr)
            : m_current_p(ptr)
        {
        }
        reference operator*() const { return m_current_p->m_student_p; }
        value_type operator->() { return m_current_p->m_student_p; }

        // Prefix increment
        iterator& operator++()
        {
            m_current_p = m_current_p->getNextStudentNode_p();
            return *this;
        }

        // Postfix increment
        iterator operator++(int)
        {
            iterator tmp = *this;
            ++(*this);
            return tmp;
        }

        friend bool operator==(const iterator& a, const iterator& b)
        {
            return a.m_current_p == b.m_current_p;
        };
        friend bool operator!=(const iterator& a, const iterator& b)
        {
            return a.m_current_p != b.m_current_p;
        };

    private:
        std::shared_ptr<StudentNode> m_current_p { nullptr };
    };

    /// Destructor
    ~StudentList();

    iterator begin() { return iterator { m_head_p }; }
    iterator end() { return iterator {}; }

    void insertStudent_p(std::shared_ptr<Student> student_p);
    void removeFirstStudent_p();
    std::shared_ptr<Student> getStudentAtIndex(size_t index) const;
    void insertStudentAtIndex(std::shared_ptr<Student> student_p, size_t index);
    size_t getNumberOfStudents() const;
};

#endif // StudentList_H
