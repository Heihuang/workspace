#include <iostream>
#include <stdlib.h>
#include <string>
#include <memory>
#include <vector>
#include <utility>
using namespace std;

class Test
{
public:
    Test() : p(nullptr){}
    ~Test()
    {
        delete p;
        p = nullptr;
        cout << "del" << endl;
    }
private:
    char * p;
};

void Move()
{
    std::string str = "Hello";
    std::vector<std::string> v;
    //调用常规的拷贝构造函数，新建字符数组，拷贝数据
    v.push_back(str);
    std::cout << "After copy, str is \"" << str << "\"\n";
    //调用移动构造函数，掏空str，掏空后，最好不要使用str
    v.push_back(std::move(str));
    std::cout << "After move, str is \"" << str << "\"\n";
    std::cout << "The contents of the vector are \"" << v[0]
              << "\", \"" << v[1] << "\"\n";
}

void Assert()
{
    //static_assert("64-bit code generation is not supported.");
}

void MT()
{
    shared_ptr<Test> ptest = make_shared<Test>();
    shared_ptr<Test> ptest2 = make_shared<Test>();
    shared_ptr<Test> ptest4 = make_shared<Test>();
    vector<shared_ptr<Test>> t;
    vector<shared_ptr<Test>> m;
    
    t.push_back(ptest);
    //t.push_back(ptest4);
    m.push_back(ptest2);
    long i = 0;
    do
    {
        
        //m.erase(m.begin());
        //  shared_ptr<Test> tt = t.front();
        //t.clear();
        //ptest.reset();
        //delete ptest.get();
        //ptest2 = ptest3;
        //auto it = m.begin();
        
        //shared_ptr<Test> ptest3 = *it;
        //cout << "use_count3# = " << ptest3.use_count() << endl; //2
       // *it = make_shared<Test>();
        //t.erase(it);
        //
        //m.clear();
        if (i == 0){
            //m.push_back(ptest3);
            //cout << "use_count3## = " << ptest3.use_count() << endl; //2
        }
        
       // (*it).reset();
        
        //ptest3.reset();
       // cout << "use_count3### = " << ptest3.use_count() << endl; //2
        swap(t, m);
        auto it = m.begin();
        m.erase(it);
        ++i;
        //ptest3.reset();
        //t.erase(it);
        //t.swap(m);
        //mm = m;
        
        //cout << "t = " << t.size() << endl;
        //cout << "m = " << m.size() << endl;
        cout << "use_count1 = " << ptest.use_count() << endl;  //2
        cout << "use_count2 = " << ptest2.use_count() << endl; //2
       // cout << "use_count3 = " << ptest3.use_count() << endl; //2
    } while (i < 2);
    cout << "use_count1 = " << ptest.use_count() << endl;  //2
    cout << "use_count2 = " << ptest2.use_count() << endl; //2
    swap(t, m);
    
}

int main(int argc, char **argv)
{
    MT();
    //Assert();
    /* std::set_terminate(__gnu_cxx::__verbose_terminate_handler);
    try
    {
        //throw 1;
    }
    catch (...)
    {
        std::terminate();
    }*/
    cout << "test" << endl;
    return 0;
}