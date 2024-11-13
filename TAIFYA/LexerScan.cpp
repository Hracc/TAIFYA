#include <fstream>

#include <iostream>

#include "lexer.h"
#include "tables.h"

bool scan() {
	states CS;
	int z = 0;
	CS = H;
	while (CS != V) {
		if (!gc()) {
			return false;
		}
		if (CH == '}') {
			out(1,2);
			CS = V;
			continue;
		}
		if (CH == '{') {
			out(1, 1);
			continue;
		}
		switch (CS) {
		case V:
			return true;
		default:
			int a = 0;
			nill();
			add();
			z = look(TL);
			if (z==0) {
				return false;
			} else
			{
				out(2, z);
			}
		}
	}
	
}