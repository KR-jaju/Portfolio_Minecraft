#include "pch.h"
#include "PipeManager.h"

PipeManager::PipeManager()
{
}

PipeManager::~PipeManager()
{
}

bool PipeManager::addPipeline(
	string name, 
	shared_ptr<Pipeline> pipeline
)
{
	if (this->checkPipelineBook(name))
		return false;
	this->pipeline_book[name] = pipeline;
	return true;
}

shared_ptr<Pipeline> PipeManager::getPipeline(string name)
{
	if (this->checkPipelineBook(name) == false)
		return nullptr;
	return this->pipeline_book[name];
}

bool PipeManager::checkPipelineBook(string name) const
{
	map<string, shared_ptr<Pipeline>>::const_iterator it;
	it = this->pipeline_book.find(name);
	if (it == this->pipeline_book.end())
		return false;
	return true;
}
