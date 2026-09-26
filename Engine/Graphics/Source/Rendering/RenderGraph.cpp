#include "Luma/Rendering/RenderGraph.h"

namespace Luma
{
    RenderGraph::Pass::Pass(String name) : m_Name(std::move(name))
    {
    }

    const String& RenderGraph::Pass::getName() const
    {
        return m_Name;
    }

    void RenderGraph::execute(CommandBuffer* cmdBuffer)
    {
        for (const auto& pass : m_Passes)
        {

        }
    }
}
