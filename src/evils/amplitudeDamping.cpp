/*
 * amplitudeDamping.cpp
 */
#include <memory>
#include <unistd.h>
#include <stdio.h>

#include "amplitudeDamping.h"
#include "matrix.h"
#include "qubit.h"

using namespace std;

namespace Quantum {
void AmplitudeDamping::doEvil(shared_ptr<Qubit> q) {
	Matrix E_AD_0(2,2);
	Matrix E_AD_1(2,2);
	int i, j;

	E_AD_0.set(0, 0, 1);
	E_AD_0.set(1, 1, sqrt(1-this->eta));
	E_AD_1.set(1, 0, sqrt(this->eta));

	Matrix rho = q->v->toDensity();
	Matrix rho_0 = Matrix::matrixMultiply(E_AD_0, rho);
	rho_0 = Matrix::matrixMultiply(rho_0, E_AD_0);

	Matrix rho_1 = Matrix::matrixMultiply(E_AD_1, rho);
	E_AD_1.set(1, 0, 0);
	E_AD_1.set(0, 1, sqrt(this->eta));
	rho_1 = Matrix::matrixMultiply(rho_1, E_AD_1);
	
	for ( i = 0; i < rho.getCols(); i++ ) {
		for ( j = 0; j < rho.getRows(); j++ ) {
			rho.set(i, j, rho_0.get(i, j) + rho_1.get(i, j));
		}
	}

	q->v->fromDensity(rho);
}

void AmplitudeDamping::setEta(float newEta) {
	this->eta = newEta;
}

float AmplitudeDamping::getEta() {
	return this->eta;
}

}
