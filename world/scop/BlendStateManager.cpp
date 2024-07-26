#include "pch.h"
#include "BlendStateManager.h"

BlendStateManager::BlendStateManager(ComPtr<ID3D11Device> device)
    : device(device)
{
}

BlendStateManager::~BlendStateManager()
{
}

bool BlendStateManager::addBlendState(const string& name)
{
    if (this->checkBlendStateBook(name))
        return false;
    shared_ptr<BlendState> blend_state;
    blend_state = make_shared<BlendState>(this->device);
    blend_state->create();
    this->blend_state_book[name] = blend_state;
    return true;
}

bool BlendStateManager::addBlendState(
    const string& name, 
    const D3D11_RENDER_TARGET_BLEND_DESC& blend_desc,
    const float& factor
)
{
    if (this->checkBlendStateBook(name))
        return false;
    shared_ptr<BlendState> blend_state;
    blend_state->create(blend_desc, factor);
    this->blend_state_book[name] = blend_state;
    return true;
}

bool BlendStateManager::removeBlendState(const string& name)
{
    if (this->checkBlendStateBook(name) == false)
        return false;
    this->blend_state_book.erase(name);
    return true;
}

shared_ptr<BlendState> BlendStateManager::getBlendState(
    const string& name
)
{
    if (this->checkBlendStateBook(name) == false)
        return nullptr;
    return this->blend_state_book[name];
}

bool BlendStateManager::checkBlendStateBook(const string& name) const
{
    map<string, shared_ptr<BlendState>>::const_iterator it;
    it = this->blend_state_book.find(name);
    if (it == this->blend_state_book.end())
        return false;
    return true;
}
