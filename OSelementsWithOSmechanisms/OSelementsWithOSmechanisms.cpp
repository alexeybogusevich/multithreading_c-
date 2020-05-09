// OSelementsWithOSmechanisms.cpp : Этот файл содержит функцию "main". Здесь начинается и заканчивается выполнение программы.
//

#include "pch.h"
#include <iostream>
#include <thread>
#include <string>
#include <chrono>
#include <future>
#include <windows.h>
#include <utility>


using namespace std;

const int F_INPUT = 3;
const int G_INPUT = 3;

int f(int x);
int g(int x);

bool fDone = false;
bool gDone = false;

int main()
{
	// Create a promise and get its future.
	//promise<int> fPromise;
	//promise<int> gPromise;

	//future<int> fResult = fPromise.get_future();
	//future<int> gResult = gPromise.get_future();

	//thread fThread(f, move(fPromise), F_INPUT);
	//thread gThread(g, move(gPromise), G_INPUT);

	auto fFuture = async(f, F_INPUT);
	auto gFuture = async(g, G_INPUT);

	cout << "Threads are now running" << endl;
	//auto th1 = async(f, F_INPUT);
	//auto th2 = async(g, G_INPUT);
	
	bool userInteraction = true;

	while (!fDone || !gDone)
	{
		Sleep(10000);
		if (userInteraction)
		{
			char decision;
			cout << "Continue execution - [1]\nWait for all the threads to finish execution - [2]\nBreak execution - [3]?" << endl;
			cin >> decision;

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
		//cout << endl << "Function F now sleeps for " << i << " seconds" << endl;
		result += i;
		this_thread::sleep_for(chrono::seconds(i));
	}

	fDone = true;
	return result;
}

int g(int x)
{
	int result = 0;

	for (int i = 2*x; i > 0; --i)
	{
		//cout << endl << "Function G now sleeps for " << i << " seconds" << endl;
		result += i;
		this_thread::sleep_for(chrono::seconds(i));
	}

	gDone = true;
	return result;
}

