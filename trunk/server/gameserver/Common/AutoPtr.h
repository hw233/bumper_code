////////////////////////////////////////////////////////////////
//文件名称：AutoPtr.h
//功能描述：智能指针
//版本说明：windows操作系统需要定义宏：__WINDOWS__
//			linux操作系统需要定义宏：__LINUX__
//编写作者：2017.09.01 yanfeng.hu
//
//修改情况：完成基本功能搭建 V1.0.0
//
////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////
//定义头文件只会被其他文件包含一次
////////////////////////////////////////////////////////////////
//#if __MSC_VER > 1000
//#pragma once
//#endif

#ifndef __AUTOPTR_H__
#define __AUTOPTR_H__

////////////////////////////////////////////////////////////////
//提示一些报错信息，暂定此文件只能使用在LINUX系统上
////////////////////////////////////////////////////////////////
#if !defined(__LINUX__)
//#error ERROR: Only In Linux Targets Supported!
#endif

////////////////////////////////////////////////////////////////
//定义字节对其
////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////
//定义C++可以读取c文件，下面的代码按照c的语言解析来编译
////////////////////////////////////////////////////////////////
/*#ifndef __cpluscplus  
extern "C"  //C++  
{  
#endif */

////////////////////////////////////////////////////////////////
//网络信息预定义宏
////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////
//Class AutoPtr
////////////////////////////////////////////////////////////////
template <typename  T>
class AutoPtr
{
public:
	AutoPtr();
	~AutoPtr();
	
	//oprator overload
public:
	/*重载->运算符*/
	T* operator -> ()	{	return _ptr;	} //访问谁的成员，就返回谁的地址

	/*重载星号运算符*/
	T& operator * ()	{	return *_ptr;	}

	/*重载<<运算符*/
	ostream& operator<<(ostream& o,const T& a);

	/*重载>>运算符*/
	ostream& operator>>(ostream& o,const T& a);

	//关系运算符重载
	bool operator == (const T& ); 
    bool operator != (const T& );
    bool operator < (const T& );
    bool operator <= (const T& );
    bool operator > (const T& );
    bool operator >= (const T& );

    //逻辑运算符重载
    bool operator || (const T& );
    bool operator && (const T& );
    bool operator ! ();

    //位运算符重载
    T operator | (const T& );
    T operator & (const T& );
    T operator ^ (const T& );
    T operator << (int i);
    T operator >> (int i);
    T operator ~ ();

    //赋值运算符重载****没有=哦。
    T& operator += (const T& );
    T& operator -= (const T& ); 
    T& operator *= (const T& );
    T& operator /= (const T& );
    T& operator %= (const T& );
    T& operator &= (const T& );
    T& operator |= (const T& );
    T& operator ^= (const T& );
    T& operator <<= (int i);
    T& operator >>= (int i);

    //内存运算符重载
    void *operator new(size_t size);
    void *operator new(size_t size, int i);
    void *operator new[](size_t size);
    void operator delete(void*p);
    void operator delete(void*p, int i, int j);
    void operator delete [](void* p);

    //特殊运算符重载
    T& operator = (const T& );
    char operator [] (int i);//返回值不能作为左值
    const char* operator () ();
    T operator -> ();

    //类型转换符
    operator bool ();
    operator char* () const;
    operator int ();
    operator const char () const;
    operator short int () const;
    operator long long () const;
    //还有很多就不写了

private:
	T* _ptr;
};

#endif

