#include "pch.h"
#include "SamplerManager.h"

SamplerManager::SamplerManager(ComPtr<ID3D11Device> device)
	: device(device)
{
}

SamplerManager::~SamplerManager()
{
}

bool SamplerManager::addSamplerState(
	const string& name, 
	const D3D11_SAMPLER_DESC& desc)
{
	if (this->checkSamplerStateBook(name))
		return false;
	shared_ptr<SamplerState> sampler_state =
		make_shared<SamplerState>(this->device);
	sampler_state->create(desc);
	this->sampler_state_book[name] = sampler_state;
	return true;
}

bool SamplerManager::addSamplerState(const string& name)
{
	if (this->checkSamplerStateBook(name))
		return false;
	shared_ptr<SamplerState> sampler_state =
		make_shared<SamplerState>(this->device);
	sampler_state->create();
	this->sampler_state_book[name] = sampler_state;
	return true;
}

bool SamplerManager::removeSamplerState(const string& name)
{
	if (this->checkSamplerStateBook(name) == false)
		return false;
	this->sampler_state_book.erase(name);
	return true;
}

shared_ptr<SamplerState> SamplerManager::getSamplerState(
	const string& name
)
{
	if (this->checkSamplerStateBook(name) == false)
		return nullptr;
	return this->sampler_state_book[name];
}

bool SamplerManager::checkSamplerStateBook(const string& name) const
{
	map<string, shared_ptr<SamplerState>>::const_iterator it;
	it = this->sampler_state_book.find(name);
	if (it == this->sampler_state_book.end())
		return false;
	return true;
}
