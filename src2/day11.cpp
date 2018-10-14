/**
   Advent of Code 2015

   --- Day 11: Corporate Policy ---

   Santa's previous password expired, and he needs
      help choosing a new one.

   To help him remember his new password after the
      old one expires, Santa has devised a method
      of coming up with a password based on the
      previous one. Corporate policy dictates that
      passwords must be exactly eight lowercase
      letters (for security reasons), so he finds
      his new password by incrementing his old
      password string repeatedly until it is
      valid.

   Incrementing is just like counting with
      numbers: xx, xy, xz, ya, yb, and so on.
      Increase the rightmost letter one step; if
      it was z, it wraps around to a, and repeat
      with the next letter to the left until one
      doesn't wrap around.

   Unfortunately for Santa, a new Security-Elf
      recently started, and he has imposed some
      additional password requirements:

    Passwords must include one increasing straight
       of at least three letters, like abc, bcd,
       cde, and so on, up to xyz. They cannot skip
       letters; abd doesn't count.
    Passwords may not contain the letters i, o, or
       l, as these letters can be mistaken for
       other characters and are therefore
       confusing.
    Passwords must contain at least two different,
       non-overlapping pairs of letters, like aa,
       bb, or zz.

   For example:

    'hijklmmn'
       meets the first requirement (because
       it contains the straight hij) but fails the
       second requirement requirement (because it
       contains i and l).
    abbceffg
       meets the third requirement (because
       it repeats bb and ff) but fails the first
       requirement.
    abbcegjk
       fails the third requirement, because
       it only has one double letter (bb).
    The next password after abcdefgh is abcdffaa.
    The next password after ghijklmn is ghjaabcc,
       because you eventually skip all the
       passwords that start with ghi..., since i
       is not allowed.

   Given Santa's current password (your puzzle
      input), what should his next password be?

 **/

 #include <string>
 #include <iostream>

using namespace std;

string
IncrementPassword(string OldPassword)
{
  string NewPassword = OldPassword;
  (NewPassword[7])++;
  for(uint32_t i = 7; i > 0; --i)
  {
    if(NewPassword[i] > 'z')
    {
      NewPassword[i] = 'a';
      NewPassword[i - 1]++;
    }
    else
    {
      break;
    }
  }
  if(NewPassword[0] > 'z')
  {
    NewPassword[0] = 'a';
  }

  return NewPassword;
}

bool
CheckIncrementingLetters(string Password)
{
  bool isValid = false;
  for (uint32_t i = 2; i < Password.size(); ++i)
  {
    char ThisLetter = Password[i];
    char OneLetterBack = Password[i - 1];
    char TwoLettersBack = Password[i - 2];

    if((ThisLetter - 1) == OneLetterBack && (OneLetterBack - 1) == TwoLettersBack)
    {
      isValid = true;
      break;
    }
  }

  return isValid;
}

bool
CheckInvalidLetters(string Password)
{
  bool isValid = false;
  if((Password.find('i')==string::npos) && (Password.find('j')==string::npos) && (Password.find('o')==string::npos))
  {
    isValid = true;
  }
  else
  {
    isValid = false;
  }

  return isValid;
}

bool
CheckRepeatedChars(string Password)
{
  bool isValid = false;
  int32_t NumberRepeats = 0;
  for(int32_t i = 1; i<Password.size(); ++i)
  {
    if(Password[i] == Password[i - 1])
    {
      NumberRepeats++;
      ++i;
    }

    if(NumberRepeats >= 2)
    {
      isValid = true;
      break;
    }
  }

  return isValid;
}

int
main(int argc,
     char *argv[])
{
  int ReturnCode;
  string OldPassword = string{argv[1]};
  if(!(argc == 2 || argc == 3))
  {
    cout << "Must include one or two arguments.\n";
    cout << "day11 <oldpassword> [d]\n";
    cout << "If 'd' is specified, debug messages will be output\n";
    ReturnCode = 1;
  }
  else if(OldPassword.size() != 8)
  {
    cout << "Password length must be exactly 8 characters.\n";
    ReturnCode = 2;
  }
  else
  {
    bool isTest = (argc == 3);
    string PrevPassword = string{OldPassword};
    string NewPassword;

    int32_t NumPasses = 0;
    bool isValid = false;
    for(bool isValid = false; isValid == false;)
    {
      NewPassword = IncrementPassword(PrevPassword);

      if(isTest)
        cout << "Incremented Password: " << NewPassword << endl;

      bool isIncrementValid = CheckIncrementingLetters(NewPassword);

      if(isTest)
        cout << "Increment Valid: " << isIncrementValid << endl;

      bool isInvalidLettersValid = CheckInvalidLetters(NewPassword);

      if(isTest)
        cout << "Invalid Letters Valid: " << isInvalidLettersValid << endl;
      bool isRepeatedCharsValid = CheckRepeatedChars(NewPassword);

      if(isTest)
        cout << "Repeated Chars Valid: " << isRepeatedCharsValid << endl;

      isValid = isInvalidLettersValid & isRepeatedCharsValid & isIncrementValid;

      if(isTest)
        cout << "Valid: " << isValid << endl;

      PrevPassword = NewPassword;

      if(++NumPasses % 100 == 0)
      {
        cout << "NumPasses: " << NumPasses << " Pass: " << NewPassword << endl;
      }

      ReturnCode = 0;
    }

    cout << "Final Password: " << NewPassword << endl;

  }



  return ReturnCode;
}
