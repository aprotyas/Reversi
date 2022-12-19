#pragma once

#include <iostream>

enum class Disk : char { Empty = ' ', Dark = 'd', Light = 'l' };

std::ostream& operator<<(std::ostream& os, const Disk disk);
