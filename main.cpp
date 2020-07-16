#include <iostream>
#include <string>
#include <cstdlib>
#include <sstream>
using namespace std;

float eval_coeff(int num, string coeff_str);
int sort_alpha(int &vars, string name, string varName[], float coeff[][101], int eqnNum);
string str_term(float coeff, string name);

int main(){
    string equations[100];          // equations array (without spaces)
    string varName[100];            // variables names array
    float coeff[100][101] {0};      // coefficients array
    int n, i, j;                    // n: number of equations, i,j counters
    int num_vars = 0;               // number of variables

    // taking the equations from the user
    cout << "Enter the number of equations: ";
    cin >> n;
    for (i = 0; i < n; i++){
        cout << "Eqn " << i+1 << ": ";
        cin >> equations[i];
    }

    // storing the data in the two arrays
    for (i = 0; i < n; i++)             // equations counter
    {
        int firstC = 0;     // order of the first char in the coefficient
        int firstV = 0;     // order of the first char in the variable name
        int numC = 0;       // number of the characters of the coefficient
        int numV = 0;       // number of the characters of the variable name
        int aftEqual = -1;  // order of the char that is after = in the equation
        char type = 'C';    // describes the current char: coefficient (C) & variable name (V)
        char check = 'C';   // to change between coefficient (C) & variable name (V)
        char place = 'B';   // before (B) or after (A) =
        float COEFF;        // coefficient value

        for (j = 0; j < equations[i].length(); j++)     // characters counter
        {
            switch (equations[i][j])
            {
                case '+':    case '-':
	                // if the constant is in the middle of the equation
	                if (type == 'C' && place == 'B' && j != 0)
	                    coeff[i][100] -= atof((equations[i].substr(firstC,numC)).c_str());
	                else if (type == 'C' && place == 'A' && j != aftEqual)
	                    coeff[i][100] += atof((equations[i].substr(firstC,numC)).c_str());
                	firstC = j;    		numC = 1;    	type = 'C';
                break;

                case '0':    case '1':    case '2':    case '3':
                case '4':    case '5':    case '6':    case '7':
                case '8':    case '9':    case '.':
                    if (type == 'C')      numC++;
                    else                  numV++;
                break;

                case '=':
	                aftEqual = j+1;
	                // if the constant is before =
	                if (type == 'C'){
	                    coeff[i][100] -= atof((equations[i].substr(firstC,numC)).c_str());
	                    firstC = j+1;          numC = 0;           place = 'A';
	                }
	                else        type = 'C';
                break;

                default:    	numV++;     	type = 'V';
            }

            // last char in the equation
            if (j == equations[i].length()-1){
                // constant
                if (type == 'C')
                    coeff[i][100] += atof((equations[i].substr(firstC,numC)).c_str());
                // variable
                else{
                    if (numV == 1){
                    	COEFF = eval_coeff(numC, equations[i].substr(firstC,numC));
                        check = 'V';                firstV = j;
                    }
                    string NAME = equations[i].substr(firstV,numV);
					int order = sort_alpha(num_vars, NAME, varName, coeff, i);
                    varName[order] = NAME;          coeff[i][order] -= COEFF;
                }
            }

            // storing coefficients and variables names
            if (type == check)              continue;
            // cut the coeff (from C to V)
            else if (type == 'V'){
                COEFF = eval_coeff(numC, equations[i].substr(firstC,numC));
                check = 'V';                firstV = j;
            }
            // cut the variable name (from V to C)
            else{
                string NAME = equations[i].substr(firstV,numV);
                int order = sort_alpha(num_vars, NAME, varName, coeff, i);
                varName[order] = NAME;      check = 'C';        numV = 0;

                if (equations[i][j] == '='){
                    coeff[i][order] += COEFF;           place = 'A';
                    firstC = j+1;                       numC = 0;
                }
                else if (place == 'B')      coeff[i][order] += COEFF;
                else                        coeff[i][order] -= COEFF;
            }
        }
    }

    bool done = false;					// = true after excuting operation D_value or solve
    float DET = 1;						// determinant value
    float values[100];					// variables solutions
    string operation;
    cin.ignore();
    while(1){
    	cout << "\nEnter an operation: ";
        getline(cin, operation);

        if (operation == "num_vars")
            cout << num_vars << endl;

        else if (operation.substr(0,8) == "equation"){
            int eqnNum = atof(operation.substr(9).c_str()) - 1;
            string newEqn;

            for (i = 0; i < num_vars; i++)
                newEqn += str_term(coeff[eqnNum][i], varName[i]);
            ostringstream strInd;
            strInd << coeff[eqnNum][100];
            newEqn += ("=" + strInd.str());
            if (newEqn[0] == '+')               newEqn.erase(0,1);
            cout << newEqn << endl;
            equations[eqnNum] = newEqn;
        }

        else if (operation.substr(0,6) == "column"){
            string column = operation.substr(7);
            for (i = 0; i < num_vars; i++)
                if (varName[i] == column)       break;
            for (j = 0; j < n; j++)
                cout << coeff[j][i] << endl;
        }

        else if (operation.substr(0,3) == "add"){
            int eqn1, eqn2;
            for (i = 5; i < operation.length(); i++)
                if (operation[i] == ' '){
                    eqn1 = atof(operation.substr(4,i-4).c_str()) - 1;
                    eqn2 = atof(operation.substr(i+1).c_str()) - 1;
                    break;
                }
            string sum;

            for (i = 0; i < num_vars; i++)
                sum += str_term(coeff[eqn1][i] + coeff[eqn2][i], varName[i]);
            ostringstream strInd;
            strInd << coeff[eqn1][100] + coeff[eqn2][100];
            sum += ("=" + strInd.str());
            if (sum[0] == '+')              sum.erase(0,1);
            cout << sum << endl;
        }

        else if (operation.substr(0,8) == "subtract"){
            int eqn1, eqn2;
            for (i = 10; i < operation.length(); i++)
                if (operation[i] == ' '){
                    eqn1 = atof(operation.substr(9,i-9).c_str()) - 1;
                    eqn2 = atof(operation.substr(i+1).c_str()) - 1;
                    break;
                }
            string diff;

            for (i = 0; i < num_vars; i++)
                diff += str_term(coeff[eqn1][i] - coeff[eqn2][i], varName[i]);
            ostringstream strInd;
            strInd << coeff[eqn1][100] - coeff[eqn2][100];
            diff += ("=" + strInd.str());
            if (diff[0] == '+')             diff.erase(0,1);
            cout << diff << endl;
        }

        else if (operation.substr(0,10) == "substitute"){
            string var;
            int subEqn, varEqn, divide = 11;    // first char of the variable and the subEqn
            for (i = 12; i < operation.length(); i++){
                if (operation[i] == ' ' && divide == 11){
                    var = operation.substr(11, i-11);
                    divide = i + 1;
                }
                else if (operation[i] == ' ' && divide != 11){
                    subEqn = atof(operation.substr(divide,i-divide).c_str()) - 1;
                    varEqn = atof(operation.substr(i+1).c_str())-1;
                    break;
                }
            }
            for (i = 0; i < num_vars; i++)
                if (varName[i] == var)      break;
            float subCoeff = -coeff[subEqn][i]/coeff[varEqn][i];
            string subs;

            for (i = 0; i < num_vars; i++)
                subs += str_term(coeff[subEqn][i] + subCoeff*coeff[varEqn][i], varName[i]);
            ostringstream strInd;
            strInd << coeff[subEqn][100] + subCoeff*coeff[varEqn][100];
            subs += ("=" + strInd.str());
            if (subs[0] == '+')                 subs.erase(0,1);
            cout << subs << endl;
        }

        else if (operation == "D")
            for (i = 0; i < n; i++){
                for (j = 0; j < num_vars; j++)
                    cout << coeff[i][j] << "\t";
                cout << endl;
            }

        else if (operation.substr(0,2) == "D "){
            string var = operation.substr(2);
            for (i = 0; i < num_vars; i++)
                if (varName[i] == var)          break;

            for (int k = 0; k < n; k++){
                for (j = 0; j < num_vars; j++){
                    if (j == i)         cout << coeff[k][100] << "\t";
                    else                cout << coeff[k][j] << "\t";
                }
                cout << endl;
            }
        }

        else if (operation == "D_value" || operation == "solve"){
            if (done == false && num_vars == n){
                float det[100][101];                        // duplicate to coeff array
                for (i = 0; i < 100; i++)
                    for (j = 0; j < 101; j++)
                        det[i][j] = coeff[i][j];

                for (i = 0; i < n-1; i++){                  // substitute from equation
                    if (det[i][i] == 0)                     // element in the diagonal = 0
                        for (j = i+1; j < n; j++){
                            if (det[j][i] == 0 && j != n-1)
                                continue;
                            else if (det[j][i] == 0 && j == n-1)
                                DET = 0;
                            else{							// swap equations
                                for (int k = i; k < n; k++){
                                    float temp = det[i][k];
                                    det[i][k] = det[j][k];
                                    det[j][k] = temp;
                                }
                                float temp = det[i][100];
                                det[i][100] = det[j][100];
                                det[j][100] = temp;
                                DET *= -1;
                                break;
                            }
                        }
                    if (DET == 0)       break;

                    for (j = i+1; j < n; j++){              // substitute to equation
                        float nom = det[j][i];              // nominator
                        for (int k = i; k < n; k++)         // coefficient counter
                            det[j][k] -= nom/det[i][i]*det[i][k];
                        det[j][100] -= nom/det[i][i]*det[i][100];
                    }
                }
                if (DET != 0){
                    for (i = 0; i < n; i++)
                        DET *= det[i][i];
	                for (i = 0; i < n; i++)
	                    values[i] = det[i][100];
	                for (i = n-1; i >= 0; i--){
	                    for (j = n-1; j > i; j--)
	                        values[i] -= det[i][j]*values[j];
	                    values[i] /= det[i][i];
                	}
                }
                done = true;
            }
            if (operation == "D_value")
                cout << DET << endl;
            else if (operation == "solve")
            	for (i = 0; i < n; i++)
                    cout << varName[i] << " = " << values[i] << endl;
        }

        else if (operation == "quit")
            break;
    }
    return 0;
}

float eval_coeff(int num, string coeff_str){
    if (num == 0)                   		return 1;
    else if (num == 1){
        if (coeff_str == "+")           	return 1;
        else if (coeff_str == "-")      	return -1;
        else								return atof(coeff_str.c_str());
    }
    else									return atof(coeff_str.c_str());
}

int sort_alpha(int &vars, string name, string varName[], float coeff[][101], int eqnNum){
    // order: the right order of the variable name sorted alphabetically
    // shift: to shift coefficients & variables names one element to the right
    // prev: to shift the stored data of the previous equations
	int order;
	for (order = 0; order < vars; order++){
        if (name.compare(varName[order]) == 0)				return order;
        else if (name.compare(varName[order]) == 1)			continue;
        else{
            for (int shift = vars; shift > order; shift--)
            {
                varName[shift] = varName[shift-1];
                for (int prev = 0; prev <= eqnNum; prev++)
                    coeff[prev][shift] = coeff[prev][shift-1];
            }
            for (int prev = 0; prev <= eqnNum; prev++)
                coeff[prev][order] = 0;
            break;
        }
    }
    vars++;                         return order;
}

string str_term(float coeff, string name){
    if (coeff == 0)                 return "";
    else if (coeff == 1)            return "+" + name;
    else if (coeff == -1)           return "-" + name;
    else {
        ostringstream strCoeff;
        strCoeff << coeff;
        if (coeff > 0)              return "+" + strCoeff.str() + name;
        else                        return strCoeff.str() + name;
    }
}
