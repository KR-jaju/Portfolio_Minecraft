#pragma once

class Pipeline;

class PipeManager
{
public:
	PipeManager();
	~PipeManager();
	bool addPipeline(string name, shared_ptr<Pipeline> pipeline);
	shared_ptr<Pipeline> getPipeline(string name);

private:
	bool checkPipelineBook(string name) const;

private:
	map<string, shared_ptr<Pipeline>> pipeline_book;
};

