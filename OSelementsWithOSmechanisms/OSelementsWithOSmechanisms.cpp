﻿// OSelementsWithOSmechanisms.cpp : Этот файл содержит функцию "main". Здесь начинается и заканчивается выполнение программы.
//

#include "pch.h"
#include <iostream>
#include <thread>
#include <string>
#include <chrono>
#include <future>
#include <windows.h>





using namespace std;

const int F_INPUT = 5;
const int G_INPUT = 10;

int f(int x);
int g(int x);

int main()
{
	// Create a promise and get its future.
	promise<bool> p;
	auto future = p.get_future();

	thread th1(f, F_INPUT);
	th1.detach();
	thread th2(g, G_INPUT);
	th2.detach();

	bool finished = false;

	while (!finished)
	{
		Sleep(10);

		auto status = future.wait_for(0ms);

		if (status == future_status::ready)
		{
			std::cout << "Threads finished" << std::endl;
			finished = true;
		}
		else
		{
			std::cout << "Threads still running" << std::endl;
		}

		if (!finished)
		{

			char decision;
			cout << "Would you like to wait for the threads to finish processing? [y/n]" << endl;
			cin >> decision;

			if (decision == 'y')
			{
				continue;
			}
			else if (decision == 'n')
			{
				return 0;
			}
			else
			{
				throw exception("Incorrect user input");
			}
		}
	}
	std::future<int> ret = std::async(&f);
}

int f(int x)
{
	int result = 0;

	for (int i = x; i > 0; --i)
	{
		//cout << "Function F now sleeps for " << i << " seconds" << endl;
		result += i;
		this_thread::sleep_for(chrono::seconds(i));
	}

	return result;
}

int g(int x)
{
	int result = 0;

	for (int i = 2*x; i > 0; --i)
	{
		//cout << "Function G now sleeps for " << i << " seconds" << endl;
		result += i;
		this_thread::sleep_for(chrono::seconds(i));
	}

	return result;
}
