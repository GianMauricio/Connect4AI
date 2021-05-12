#include <SFML/Graphics.hpp>
#include "BaseRunner.h"
int main()
{
    BaseRunner runner;
	srand(time(NULL));
	runner.run();
}