#include <iostream>
#include <vector>
#include <random>
#include <string>

// `BigIntegerLibrary.hh' includes all of the library headers.
#include "../BigInt/BigIntegerLibrary.hh"

// adapted from the original processing source code found at:
//   https://bitbucket.org/BWerness/voxel-automata-terrain/src/master/

using std::cout;
using std::endl;

class voxel_automata_terrain
{
	public:
		voxel_automata_terrain(int levels_deep, float flip_p, std::string rule, int initmode, float lamb, float bet, float mg)
			:L(levels_deep),
			 K((1 << levels_deep) + 1),
			 flipP(flip_p),
			 lambda(lamb),
			 beta(bet),
			 mag(mg)
		{
			// resize the cubeRule
			cubeRule.resize(9);
			for(auto & x : cubeRule)
				x.resize(9);
			// initialize with zeroes
			for(auto & x : cubeRule)
				for(auto & y : x)
					y = 0;


			// resize the faceRule
			faceRule.resize(7);
			for(auto & x : faceRule)
				x.resize(7);
			// initialize with zeroes
			for(auto & x : faceRule)
				for(auto & y : x)
					y = 0;


			// resize the edgeRule
			edgeRule.resize(7);
			for(auto & x : edgeRule)
				x.resize(7);
			// initialize with zeroes
			for(auto & x : edgeRule)
				for(auto & y : x)
					y = 0;


			// resize the state
			state.resize(K);
			for(auto & x : state)
			{
				x.resize(K);
				for(auto & y : x)
					y.resize(K);
			}
			// initialize with zeroes
			for(auto & x : state)
				for(auto & y : x)
					for(auto & z : y)
						z = 0;

			// interpreting rule input
			if(rule == std::string("r"))
			{
				randomRule();
			}
			else if(rule == std::string("i"))
			{
				randomIsingRule();
			}
			else
			{
				// interpret as shortrule
				readShortRule(rule);
			}

			initState(initmode);
			evalState();


			// dumpState();
			// dumprules();
			// std::string temp = makeShortRule();
			// readShortRule(temp);
			// dumprules();

		}

		std::string getShortRule()
		{
			std::cout << makeShortRule() << std::endl;
			return makeShortRule();
		}

		// I need to be able to access this externally, to create the OpenGL texture
		std::vector<std::vector<std::vector<int>>> state;

	private:
		int L; // levels of depth, from the original code, used to compute the edge length
		int K; //  the edge length, K = (1 << L) + 1
		float flipP; // nonzero value adds stochastic behavior

		std::vector<std::vector<int>> cubeRule;
		std::vector<std::vector<int>> faceRule;
		std::vector<std::vector<int>> edgeRule;

		void dumpState()
		{
			for(auto & x : state)
			{
				for(auto & y : x)
				{
					for(auto & z : y)
					{
						std::cout << z << " ";
					}
					std::cout << std::endl;
				}
				std::cout << std::endl;
			}
		}

		// fill the center of a cube
		void evalCube(int i, int j, int k, int w)
		{
			if ((i < 0) || (j < 0) || (k < 0) || (i+w >= K) || (j+w >= K) || (k+w >= K)) return;

			int idx1 = (state[i][j][k]==1?1:0) + (state[i+w][j][k]==1?1:0) + (state[i][j+w][k]==1?1:0) + (state[i+w][j+w][k]==1?1:0) +
						(state[i][j][k+w]==1?1:0) + (state[i+w][j][k+w]==1?1:0) + (state[i][j+w][k+w]==1?1:0) + (state[i+w][j+w][k+w]==1?1:0);
			int idx2 = (state[i][j][k]==2?1:0) + (state[i+w][j][k]==2?1:0) + (state[i][j+w][k]==2?1:0) + (state[i+w][j+w][k]==2?1:0) +
						(state[i][j][k+w]==2?1:0) + (state[i+w][j][k+w]==2?1:0) + (state[i][j+w][k+w]==2?1:0) + (state[i+w][j+w][k+w]==2?1:0);

			state[i+w/2][j+w/2][k+w/2] = cubeRule[idx1][idx2];

			if ((random(1.0) < flipP) && (state[i+w/2][j+w/2][k+w/2] != 0))
			{
				state[i+w/2][j+w/2][k+w/2] = 3 - state[i+w/2][j+w/2][k+w/2];
			}
		}

		// fill a face
		void f1(int i, int j, int k, int w)
		{
			if ((i < 0) || (j < 0) || (k-w/2 < 0) || (i+w >= K) || (j+w >= K) || (k+w/2 >= K)) return;

			int idx1 = (state[i][j][k]==1?1:0) + (state[i+w][j][k]==1?1:0) + (state[i][j+w][k]==1?1:0) + (state[i+w][j+w][k]==1?1:0) +
						(state[i+w/2][j+w/2][k-w/2]==1?1:0) + (state[i+w/2][j+w/2][k+w/2]==1?1:0);
			int idx2 = (state[i][j][k]==2?1:0) + (state[i+w][j][k]==2?1:0) + (state[i][j+w][k]==2?1:0) + (state[i+w][j+w][k]==2?1:0) +
						(state[i+w/2][j+w/2][k-w/2]==2?1:0) + (state[i+w/2][j+w/2][k+w/2]==2?1:0);

			state[i+w/2][j+w/2][k] = faceRule[idx1][idx2];

			if ((random(1.0) < flipP) && (state[i+w/2][j+w/2][k] != 0))
			{
				state[i+w/2][j+w/2][k] = 3 - state[i+w/2][j+w/2][k];
			}
		}

		// fill a face
		void f2(int i, int j, int k, int w)
		{
			if ((i < 0) || (j-w/2 < 0) || (k < 0) || (i+w >= K) || (j+w/2 >= K) || (k+w >= K)) return;

			int idx1 = (state[i][j][k]==1?1:0) + (state[i+w][j][k]==1?1:0) + (state[i][j][k+w]==1?1:0) + (state[i+w][j][k+w]==1?1:0) +
						(state[i+w/2][j-w/2][k+w/2]==1?1:0) + (state[i+w/2][j+w/2][k+w/2]==1?1:0);
			int idx2 = (state[i][j][k]==2?1:0) + (state[i+w][j][k]==2?1:0) + (state[i][j][k+w]==2?1:0) + (state[i+w][j][k+w]==2?1:0) +
						(state[i+w/2][j-w/2][k+w/2]==2?1:0) + (state[i+w/2][j+w/2][k+w/2]==2?1:0);

			state[i+w/2][j][k+w/2] = faceRule[idx1][idx2];

			if ((random(1.0) < flipP) && (state[i+w/2][j][k+w/2] != 0))
			{
				state[i+w/2][j][k+w/2] = 3 - state[i+w/2][j][k+w/2];
			}
		}

		// fill a face
		void f3(int i, int j, int k, int w)
		{
			if ((i-w/2 < 0) || (j < 0) || (k < 0) || (i+w/2 >= K) || (j+w >= K) || (k+w >= K)) return;

			int idx1 = (state[i][j][k]==1?1:0) + (state[i][j][k+w]==1?1:0) + (state[i][j+w][k]==1?1:0) + (state[i][j+w][k+w]==1?1:0) +
						(state[i-w/2][j+w/2][k+w/2]==1?1:0) + (state[i+w/2][j+w/2][k+w/2]==1?1:0);
			int idx2 = (state[i][j][k]==2?1:0) + (state[i][j][k+w]==2?1:0) + (state[i][j+w][k]==2?1:0) + (state[i][j+w][k+w]==2?1:0) +
						(state[i-w/2][j+w/2][k+w/2]==2?1:0) + (state[i+w/2][j+w/2][k+w/2]==2?1:0);

			state[i][j+w/2][k+w/2] = faceRule[idx1][idx2];

			if ((random(1.0) < flipP) && (state[i][j+w/2][k+w/2] != 0))
			{
				state[i][j+w/2][k+w/2] = 3 - state[i][j+w/2][k+w/2];
			}
		}

		// fill a face
		void f4(int i, int j, int k, int w)
		{
			f1(i,j,k+w,w);
		}

		// fill a face
		void f5(int i, int j, int k, int w)
		{
			f1(i,j+w,k,w);
		}

		// fill a face
		void f6(int i, int j, int k, int w)
		{
			f1(i+w,j,k,w);
		}

		// fill every face
		void evalFaces(int i, int j, int k, int w)
		{
			f1(i,j,k,w);
			f2(i,j,k,w);
			f3(i,j,k,w);
			f4(i,j,k,w);
			f5(i,j,k,w);
			f6(i,j,k,w);
		}

		// fill an edge
		void e1(int i, int j, int k, int w)
		{
			if ((i < 0) || (j-w/2 < 0) || (k-w/2 < 0) || (i+w >= K) || (j+w/2 >= K) || (k+w/2 >= K)) return;

			int idx1 = (state[i][j][k]==1?1:0) + (state[i+w][j][k]==1?1:0) + (state[i+w/2][j-w/2][k]==1?1:0) + (state[i+w/2][j+w/2][k]==1?1:0) +
						(state[i+w/2][j][k+w/2]==1?1:0) + (state[i+w/2][j][k-w/2]==1?1:0);
			int idx2 = (state[i][j][k]==2?1:0) + (state[i+w][j][k]==2?1:0) + (state[i+w/2][j-w/2][k]==2?1:0) + (state[i+w/2][j+w/2][k]==2?1:0) +
						(state[i+w/2][j][k+w/2]==2?1:0) + (state[i+w/2][j][k-w/2]==2?1:0);

			state[i+w/2][j][k] = edgeRule[idx1][idx2];

			if ((random(1.0) < flipP) && (state[i+w/2][j][k] != 0))
			{
				state[i+w/2][j][k] = 3 - state[i+w/2][j][k];
			}
		}

		// fill an edge
		void e2(int i, int j, int k, int w)
		{
			e1(i,j+w,k,w);
		}

		// fill an edge
		void e3(int i, int j, int k, int w)
		{
			e1(i,j,k+w,w);
		}

		// fill an edge
		void e4(int i, int j, int k, int w)
		{
			e1(i,j+w,k+w,w);
		}

		// fill an edge
		void e5(int i, int j, int k, int w)
		{
			e1(i-w/2,j+w/2,k,w);
		}

		// fill an edge
		void e6(int i, int j, int k, int w)
		{
			e1(i+w/2,j+w/2,k,w);
		}

		// fill an edge
		void e7(int i, int j, int k, int w)
		{
			e1(i-w/2,j+w/2,k+w,w);
		}

		// fill an edge
		void e8(int i, int j, int k, int w)
		{
			e1(i+w/2,j+w/2,k+w,w);
		}

		// fill all edges
		void evalEdges(int i, int j, int k, int w)
		{
			e1(i,j,k,w);
			e2(i,j,k,w);
			e3(i,j,k,w);
			e4(i,j,k,w);
			e5(i,j,k,w);
			e6(i,j,k,w);
			e7(i,j,k,w);
			e8(i,j,k,w);
		}


		// parameters for the random rules
		float lambda; //  = 0.35;
		float beta; //  = 0.5;
		float mag; // = 0.0;

		// create a random rule with density lambda of filled states
		void randomRule() // parameterized by lambda
		{
			for (int i = 0; i < 9; i++)
			{
				for (int j = 0; j < 9-i; j++)
				{
					if (((i == 0) && (j == 0)) || (random(1.0)>lambda)) cubeRule[i][j] = 0;
					else cubeRule[i][j] = int(random(2))+1;
				}
			}
			for (int i = 0; i < 7; i++)
			{
				for (int j = 0; j < 7-i; j++)
				{
					if (((i == 0) && (j == 0)) || (random(1.0)>lambda)) faceRule[i][j] = 0;
					else faceRule[i][j] = int(random(2))+1;
				}
			}
			for (int i = 0; i < 7; i++)
			{
				for (int j = 0; j < 7-i; j++)
				{
					if (((i == 0) && (j == 0)) || (random(1.0)>lambda)) edgeRule[i][j] = 0;
					else edgeRule[i][j] = int(random(2))+1;
				}
			}
			// println(makeShortRule());
		}

		// create a rule based on a sample from the ising model where each one is selected with a certain weight depending on similarity to neighbors
		void randomIsingRule() // beta and mag are parameters
		{
			for (int i = 0; i < 9; i++)
			{
				for (int j = 0; j < 9-i; j++)
				{
					float f0 = exp(beta*(8-(i+j)));
					float f1 = exp(beta*i+mag);
					float f2 = exp(beta*j+mag);
					float s = (f0+f1+f2);
					float r = random(s);
					if (r < f0) cubeRule[i][j] = 0;
					else if (r < f0+f1) cubeRule[i][j] = 1;
					else cubeRule[i][j] = 2;
				}
			}
			for (int i = 0; i < 7; i++)
			{
				for (int j = 0; j < 7-i; j++)
				{
					float f0 = exp(beta*(6-(i+j)));
					float f1 = exp(beta*i+mag);
					float f2 = exp(beta*j+mag);
					float s = (f0+f1+f2);
					float r = random(s);
					if (r < f0) faceRule[i][j] = 0;
					else if (r < f0+f1) faceRule[i][j] = 1;
					else faceRule[i][j] = 2;
				}
			}
			for (int i = 0; i < 7; i++)
			{
				for (int j = 0; j < 7-i; j++)
				{
					float f0 = exp(beta*(6-(i+j)));
					float f1 = exp(beta*i+mag);
					float f2 = exp(beta*j+mag);
					float s = (f0+f1+f2);
					float r = random(s);
					if (r < f0) edgeRule[i][j] = 0;
					else if (r < f0+f1) edgeRule[i][j] = 1;
					else edgeRule[i][j] = 2;
				}
			}
			// println(makeShortRule());
		}


		void dumprules()
		{
			for (int i = 0; i < 9; i++)
			{
				for (int j = 0; j < 9; j++)
				{
					cout << cubeRule[i][j];
					cout << " ";
				}
				cout << endl;
			}

			cout << endl;

			for (int i = 0; i < 7; i++)
			{
				for (int j = 0; j < 7; j++)
				{
					cout << edgeRule[i][j];
					cout << " ";
				}
				cout << endl;
			}

			cout << endl;

			for (int i = 0; i < 7; i++)
			{
				for (int j = 0; j < 7; j++)
				{
					cout << faceRule[i][j];
					cout << " ";
				}
				cout << endl;
			}
		}


		// fill the bottom with random bits (the other sides are all empty)
		void initState(int initmode)
		{
		// fill just the bottom so we can see
			// print("Randomizing IC...");
			for (int i = 0; i < K; i++)
			{
				for (int j = 0; j < K; j++)
				{
					switch (initmode) {
						case 0: state[j][0][i] = 0;                break; // fill with zeroes
						case 1: state[j][0][i] = 1;                break; // fill with ones
						case 2: state[j][0][i] = 2;                break; // fill with twos
						case 3: state[j][0][i] = int(random(2))+1; break; // fill with random numbers [0-2 inclusive]
					}

				}
			}
		}

		void evalState()
		{
			// print("Computing...");
			// do everything on all scales in order
			for (int w = K-1; w >= 2; w /= 2)
			{
				for (int i = 0; i < K-1; i+=w)
				{
					for (int j = 0; j < K-1; j+=w)
					{
						for (int k = 0; k < K-1; k+=w)
						{
							evalCube(i,j,k,w);
						}
					}
				}
				for (int i = 0; i < K-1; i+=w)
				{
					for (int j = 0; j < K-1; j+=w)
					{
						for (int k = 0; k < K-1; k+=w)
						{
							evalFaces(i,j,k,w);
						}
					}
				}
				for (int i = 0; i < K-1; i+=w)
				{
					for (int j = 0; j < K-1; j+=w)
					{
						for (int k = 0; k < K-1; k+=w)
						{
							evalEdges(i,j,k,w);
						}
					}
				}
			}
			// draw the dots to the PShape for efficiency
			// print("Lighting...");
			// // uniDirGI();
			// addAmbient();
			// print("Building Textures...");
			// makeTexes();
			// println("Done.");
		}


		// return the string version of the rule
		std::string makeShortRule() {
			// first make a big number
			BigInteger temp = 0;
			for (int i = 0; i < 9; i++) {
				for (int j = 0; j < 9-i; j++) {
					// temp = temp.multiply(BigInteger.valueOf(3));
					// temp = temp.add(BigInteger.valueOf(cubeRule[i][j]));

					temp = temp * 3;
					temp = temp + cubeRule[i][j];
				}
			}
			for (int i = 0; i < 7; i++) {
				for (int j = 0; j < 7-i; j++) {
					// temp = temp.multiply(BigInteger.valueOf(3));
					// temp = temp.add(BigInteger.valueOf(faceRule[i][j]));

					temp = temp * 3;
					temp = temp + faceRule[i][j];
				}
			}
			for (int i = 0; i < 7; i++) {
				for (int j = 0; j < 7-i; j++) {
					// temp = temp.multiply(BigInteger.valueOf(3));
					// temp = temp.add(BigInteger.valueOf(edgeRule[i][j]));

					temp = temp * 3;
					temp = temp + edgeRule[i][j];
				}
			}
			// then expand in base 62 = 2*26+10

			std::string out = "";
			while (temp != 0) {
				// out += base62(temp.mod(BigInteger.valueOf(62)).intValue());
				// temp = temp.divide(BigInteger.valueOf(62));

				out += base62(BigInteger(temp % 62).toInt());
				temp = temp / 62;
			}
			return out;
		}


		// load the rule from a string
		void readShortRule(std::string in) {
			// first make a big number
			BigInteger temp = 0;
			for (int i = in.length()-1; i >= 0; i--) {
				temp = temp * 62;
				temp = temp + (base62(in.at(i)));
			}

			// then re-expand into base 3 and use it
			for (int i = 6; i >= 0; i--) {
				for (int j = 6-i; j >= 0; j--) {
					// edgeRule[i][j] = temp.mod(BigInteger.valueOf(3)).intValue();
					// temp = temp.divide(BigInteger.valueOf(3));

					edgeRule[i][j] = modexp(temp, 1, 3).toInt();
					temp = temp / 3;
				}
			}
			for (int i = 6; i >= 0; i--) {
				for (int j = 6-i; j >= 0; j--) {
					// faceRule[i][j] = temp.mod(BigInteger.valueOf(3)).intValue();
					// temp = temp.divide(BigInteger.valueOf(3));

					faceRule[i][j] = modexp(temp, 1, 3).toInt();
					temp = temp / 3;
				}
			}
			for (int i = 8; i >= 0; i--) {
				for (int j = 8-i; j >= 0; j--) {
					// cubeRule[i][j] = temp.mod(BigInteger.valueOf(3)).intValue();
					// temp = temp.divide(BigInteger.valueOf(3));

					cubeRule[i][j] = modexp(temp, 1, 3).toInt();
					temp = temp / 3;
				}
			}
		}


		// turn an int into base 62 version
		char base62(int in)
		{
			if (in < 10) return char(in+48);
			if (in < 36) return char((in-10)+97);
			return char((in-36)+65);
		}

		// turn a char into base 62 version
		int base62(char in)
		{
			if (in < 58) return int(in) - int('0');
			if (in < 91) return int(in) - int('A') + 36;
			return int(in) - int('a') + 10;
		}

		float random(float max)
		{
			static std::random_device rd;  //Will be used to obtain a seed for the random number engine
			static std::mt19937 gen(rd()); //Standard mersenne_twister_engine seeded with rd()
			std::uniform_real_distribution<float> dis(0.0, max);

			return dis(gen);
		}

		double random(double max)
		{
			static std::random_device rd;
			static std::mt19937 gen(rd());
			std::uniform_real_distribution<double> dis(0.0, max);

			return dis(gen);
		}

		int random(int max)
		{
			static std::random_device rd;
			static std::mt19937 gen(rd()); // this is done to match the way processing does integer rng
			std::uniform_int_distribution<int> dis(0, max-1); // https://processing.org/reference/random_.html

			return dis(gen);
		}

};

// int main(int argc, char** argv)
// {
// 	voxel_automata_terrain v(6, 0.0);

// 	std::cout << "hello" << std::endl;
// 	return 0;
// }
