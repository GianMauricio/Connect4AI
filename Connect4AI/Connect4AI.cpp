#include <SFML/Graphics.hpp>
#include "BaseRunner.h"
#include <iostream>
int main()
{
	BaseRunner runner;
	runner.run();	
}
/*
 * Known Issues:
 * 1. Ai nukes itself sometimes
 * 2. Ai seems to like left more
 * 3. AI doesn't see enemy wins...?
 * 4. Problem Areas --> AI
 *		A. Board Eval
 *			a. Check sequence functions
 *			b. Factor valuation?
 *		B. Alpha-beta recursion
 *			a. Deep review this shit
 *		C. Board done return
 *			a. Maybe expand it...?
 *			b. Make more explicit
 *		D. Test multiple Depths?
 */