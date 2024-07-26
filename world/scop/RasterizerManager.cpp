#include "pch.h"
#include "RasterizerManager.h"

RasterizerManager::RasterizerManager(ComPtr<ID3D11Device> device)
	: device(device)
{
}

RasterizerManager::~RasterizerManager()
{
}

bool RasterizerManager::addRasterizerState(
	const string& name, 
	const D3D11_FILL_MODE& fill_mode, 
	const D3D11_CULL_MODE& cull_mode
)
{
	if (this->checkRasterizerStateBook(name))
		return false;
	shared_ptr<RasterizerState> rasterizer_state;
	rasterizer_state = make_shared<RasterizerState>(this->device);
	rasterizer_state->create(fill_mode, cull_mode);
	return true;
}

bool RasterizerManager::removeRasterizerState(const string& name)
{
	if (this->checkRasterizerStateBook(name) == false)
		return false;
	this->rasterizer_state_book.erase(name);
	return true;
}

shared_ptr<RasterizerState> RasterizerManager::getRasterizerState(
	const string& name
)
{
	if (this->checkRasterizerStateBook(name) == false)
		return nullptr;
	return this->rasterizer_state_book[name];
}

bool RasterizerManager::checkRasterizerStateBook(
	const string& name
) const
{
	map<string, shared_ptr<RasterizerState>>::const_iterator it;
	it = this->rasterizer_state_book.find(name);
	if (it == this->rasterizer_state_book.end())
		return false;
	return true;
}
