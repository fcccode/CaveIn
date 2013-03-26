#ifndef _AUDIO_RENDERABLE_
#define _AUDIO_RENDERABLE_

class AudioRenderable
{
public:
	virtual void RenderAudio(const float deltaTime) = 0;
	virtual bool IsOk() const = 0;
	virtual ~AudioRenderable(){}

};
#endif