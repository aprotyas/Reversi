#pragma once

#include <iostream>

enum class Disk : char { Empty = ' ', Black = 'b', White = 'w' };

std::ostream& operator<<(std::ostream& os, const Disk disk);
