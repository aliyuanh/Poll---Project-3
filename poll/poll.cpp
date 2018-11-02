#include <iostream>
#include <string>
#include <cassert>
using namespace std;
bool isValidUppercaseStateCode(string stateCode);
bool checksSyntaxForOne(string oneState);
bool hasProperSyntax(string pollData);
int tallySeats(string pollData, char party, int& seatTally);
//*************************************
//  isValidUppercaseStateCode
//*************************************
int main() {
	assert(hasProperSyntax("CT5D"));
	assert(!hasProperSyntax("CH5D"));
	assert(!hasProperSyntax("CT5L"));
	assert(hasProperSyntax(""));
	assert(hasProperSyntax("CT"));
	assert(!hasProperSyntax("CT5D10"));
	assert(!hasProperSyntax("CT7D, , , , , CA4D"));
	assert(!hasProperSyntax("CA999D"));
	assert(!hasProperSyntax("CA99DR"));
	int seats = -999;
	assert(tallySeats("CZ95D,CT9R50D", 'd', seats) == 1 && seats == -999);
	assert(tallySeats("CA95D,CT9R50D", '%', seats) == 2 && seats == -999);
	assert(tallySeats("", 'I', seats)==0 && seats == 0);
	assert(tallySeats("CA95D", 'd', seats) == 0 && seats == 95);
	assert(tallySeats("CA95D,CT9R50D", 'd', seats) == 0 && seats == 145);
	assert(tallySeats("CA95D,CT9R50D", 'I', seats) == 0 && seats == 0);

}
// Return true if the argument is a two-uppercase-letter state code, or
// false otherwise.
bool isValidUppercaseStateCode(string stateCode)
{
	const string codes =
		"AL.AK.AZ.AR.CA.CO.CT.DE.FL.GA.HI.ID.IL.IN.IA.KS.KY."
		"LA.ME.MD.MA.MI.MN.MS.MO.MT.NE.NV.NH.NJ.NM.NY.NC.ND."
		"OH.OK.OR.PA.RI.SC.SD.TN.TX.UT.VT.VA.WA.WV.WI.WY";
	return (stateCode.size() == 2 &&
		stateCode.find('.') == string::npos  &&  // no '.' in stateCode
		codes.find(stateCode) != string::npos);  // match found
}

//checks whether or not a chunk is valid 
bool checksSyntaxForOne(string oneState) {
	//if there are multiple commas after each other, it should return false
	if (oneState.size() <2) {
		return false;
	}
	//turns the first two characters uppercase in a string called stateCode
	string stateCode = oneState.substr(0, 2);
	stateCode[0] = toupper(stateCode[0]);
	stateCode[1] = toupper(stateCode[1]);
	if (isValidUppercaseStateCode(stateCode) == false) {
		//cerr << "was not a valid state code" << endl;
		return false;
	}

	//declares string rest to be the chunk without the state code
	string rest = oneState.substr(2);
	int numbersInRow = 0;
	int numLettersInRow = 0;
	for (int i = 0; i < rest.size(); i++) {
		//checks if it's a number between 0 and 9
		if (rest[i] >= 48 && rest[i] <= 57) {
			//cerr << "between 0 and 9" << endl;
			numbersInRow++;
			//makes sure there can't be a 3 digit long number of votes in any state 
			if (numbersInRow >= 3) {
				return false;
			}
			numLettersInRow = 0;
			//a state should not be able to end with a number
			if (i == rest.size() - 1) {
				return false;
			}
		}
		//checks if it's D,d,I,i,R,r using ASCII 
		else if (rest[i] == 68 || rest[i] == 68 + 32 || rest[i] == 73 || rest[i] == 73 + 32 || rest[i] == 82 || rest[i] == 82 + 32) {
			//make sure it can't be 3 digits long 
			numbersInRow = 0;
			numLettersInRow++;
			//makes sure there needs to be a number in between the letters of each party code
			if (numLettersInRow >= 2) {
				return false;
			}
		}
		else {
			//cerr << "invalid character is " <<rest[i]<< endl;
			return false;
		}
	}
	return true;
}
//checks if the whole string has proper syntax
bool hasProperSyntax(string pollData) {
	string data = pollData;
	if (pollData.size() == 0) {
		return true;
	}
	//counts the number of commas in a string 
	int countCommas = 0;
	for (int i = 0; i < pollData.size(); i++) {
		if (pollData[i] == ',') {
			countCommas++;
		}
	}
	//goes through the pollData, sending chunks separated by commas to checksSyntaxForOne(one)
	while (countCommas >= 0) {
		string one = data.substr(0, data.find(','));
		if (checksSyntaxForOne(one) == false) {
			return false;
		}
		else {
			countCommas--;
			//assume checksSyntaxForOne is true, so we can go on to the next string
			data = data.substr(data.find(',') + 1);
		}
	}
	return true;
}

int tallySeats(string pollData, char party, int& seatTally) {
	cerr << pollData << endl;
	//checks if pollData has proper syntax
	if (hasProperSyntax(pollData) == false) {
		return 1;
	}
	//checks if the party is alphanumeric
	else if (!((party >= 65 && party <= 93) || (party >= 97 && party <= 122))) {
		return 2;
	}
	//tallys up the seats
	else {
		int seats = 0;
		string data = pollData;
		int numParty = 0;
		int partyIndex = -9999;
		for (int i = 0; i < data.size(); i++) {
			//checks if there is a party character or its upper/lowercase value 
			if (data[i] == party || data[i] == party + 32 || data[i] == party - 32) {
				numParty++;
				partyIndex = i;
				int checkIndex = i - 1;
				int numBack = 0;
				bool found = false;
				//loops backwards through the string from party character to find each number
				while (found == false) {
					if (!isdigit(data[checkIndex])) {
						found = true;
					}
					else {
						numBack++;
						checkIndex--;
					}
				}
				//isolates the string based on the index of the party character and how many digits back it is
				string num = data.substr(checkIndex + 1, numBack);
				//if num would be over 3 digits, it does not count.
				if (stoi(num) > 99) {
					num = "0";
				}
				seats += stoi(num);
			}
		}
		seatTally = seats;
	}
	return 0;
}


