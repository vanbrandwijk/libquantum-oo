/*
 * entangledRegister.cpp
 */
#include <stdio.h>
#include <math.h>
#include "entanglement.h"
#include "entangledRegister.h"
#include "register.h"
#include "types.h"

namespace Quantum {
EntangledRegister::EntangledRegister(MAX_UNSIGNED init, int width, Entanglement* ent):
Register(init, width)
{
	int i;
	this->ent = ent;
	this->opHistory = new vector<Matrix>* [width];
	for ( i = 0; i < width; i++ ) {
		this->opHistory[i] = new vector<Matrix>;
	}
}

void EntangledRegister::apply2x2Matrix(int target, Matrix *m) {
	Register::apply2x2Matrix(target, m);
	if ( ent->isEntangled(target) ) {
		this->opHistory[target]->push_back(*m);
	}
}

int EntangledRegister::measure(int target, bool preserve) {
	int result = Register::measure(target, preserve);

	this->ent->measured(this->aleph, target, result);

	return result;
}

vector<Matrix>* EntangledRegister::getOpHistory(int target) {
	return this->opHistory[target];
}

bool EntangledRegister::getAleph() {
	return this->aleph;
}

void EntangledRegister::setAleph(bool aleph) {
	this->aleph = aleph;
} 

void EntangledRegister::updateAmplitudes(int target, float p0, float p1) {
	int i;
	printf("Updating Amps.... %f, %f\n", p0, p1);
	for ( i = 0; i < this->size; i++ ) {
		if ( (int)(( this->node[i]->getState() >> (target) ) % 2 ) == 0 ) {
			this->node[i]->setAmplitude( 
				this->node[i]->getAmplitude() * sqrt(p0) );
		} else {
			this->node[i]->setAmplitude(
				this->node[i]->getAmplitude() * sqrt(p1) );
		}
	}
	this->normalize();
}

void EntangledRegister::revert(int target) {
	int i;
	printf("Reverting...\n");
	if ( !this->opHistory[target]->empty() ) {
		for ( i = this->opHistory[target]->size() - 1; i >= 0; i-- ) {
			Matrix im = Matrix::inverse(opHistory[target]->at(i));
			im.print();
			Register::apply2x2Matrix(target, &im);
		}
	}
}

void EntangledRegister::replay(int target) {
	int i;
	printf("Replaying...\n");
	for ( i = 0; i < this->opHistory[target]->size(); i++ ) {
		Register::apply2x2Matrix(target, &this->opHistory[target]->at(i));
	}
}

void EntangledRegister::playAltHistory(int target, vector<Matrix>* altHistory) {
	int i;
	printf("Playing alts...\n");
	for ( i = 0; i < altHistory->size(); i++ ) {
		altHistory->at(i).print();
		Register::apply2x2Matrix(target, &altHistory->at(i));
	}
}

void EntangledRegister::revertAltHistory(int target, vector<Matrix>* altHistory) {
	int i;
	for ( i = altHistory->size() - 1; i >= 0; i-- ) {
		Matrix im = Matrix::inverse(altHistory->at(i));
		Register::apply2x2Matrix(target, &im);
	}
}
}
