// OSelementsWithOSmechanisms.cpp : Этот файл содержит функцию "main". Здесь начинается и заканчивается выполнение программы.
//

#include "pch.h"
#include <iostream>
#include <thread>
#include <string>
#include <chrono>
#include <future>
#include <windows.h>
#include <mutex>
#include <sstream>


using namespace std;

const int F_INPUT = 4;
const int G_INPUT = 3;

int f(int x);
int g(int x);

bool fDone = false;
bool gDone = false;

bool ready = true;
condition_variable cv;

mutex m;

int main()
{
	auto fFuture = async(f, F_INPUT);
	auto gFuture = async(g, G_INPUT);

	cout << "Threads are now running...\n" << endl;
	
	bool userInteraction = true;

	while (!fDone || !gDone)
	{
		if (userInteraction)
		{
			Sleep(10000);
			unique_lock<mutex> lk(m);
			ready = false;
			char decision;
			cout << "Continue execution - [1]\nWait for all the threads to finish execution - [2]\nBreak execution - [3]?" << endl;
			cin >> decision;
			ready = true;
			cv.notify_all();
			lk.unlock();

			if (decision == '1')
			{
				continue;
			}
			else if (decision == '2')
			{
				userInteraction = false;
			}
			else if (decision == '3')
			{
				return 0;
			}
			else
			{
				throw exception("Incorrect user input");
			}
		}
	}

	bool result = fFuture.get() || gFuture.get();
	cout << endl << "f(x) || g(x) = " << result << endl;

	return 0;
}

int f(int x)
{
	int result = 0;

	for (int i = x; i > 0; --i)
	{
		stringstream ss;
		ss << this_thread::get_id();
		string message = "Function F now sleeps for " + to_string(i) + " seconds. [thread id = " + ss.str() + "]\n";
		cout << message;
		result += i;
		this_thread::sleep_for(chrono::seconds(i));
		if (!ready)
		{
			std::unique_lock<mutex> lk(m);
			cv.wait(lk, [] {return ready; });
			lk.unlock();
		}
	}

	cout << "Function F has finished its execution.\n";
	
	fDone = true;
	return result;
}

int g(int x)
{
	int result = 0;

	for (int i = 2*x; i > 0; --i)
	{
		stringstream ss;
		ss << this_thread::get_id();
		string message = "Function G now sleeps for " + to_string(i) + " seconds. [thread id = " + ss.str() + "]\n";
		cout << message;
		result += i;
		this_thread::sleep_for(chrono::seconds(i));
		if (!ready)
		{
			std::unique_lock<mutex> lk(m);
			cv.wait(lk, [] {return ready; });
			lk.unlock();
		}
	}

	cout << "Function G has finished its execution.\n";

	gDone = true;
	return result;
}

