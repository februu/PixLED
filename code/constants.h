// Days in month
const byte daysInMonth[] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};

bool isLeapYear(int year) {
  return ((year % 4 == 0) && (year % 100 != 0)) || (year % 400 == 0);
}