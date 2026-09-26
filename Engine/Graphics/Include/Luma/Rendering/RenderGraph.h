#pragma once
#include "Luma/Containers/HashMap.h"
#include "Luma/Containers/String.h"
#include "RenderGraph.h"
#include "Forward.h"
#include "Luma/Memory/Ref.h"

namespace Luma
{
    class RenderGraph
    {
    public:
        RenderGraph() = default;

        class PassBuilder
        {
        public:
            PassBuilder(RenderGraph& graph) : m_Graph(graph){}

        private:
            RenderGraph& m_Graph;
        };

        class Pass
        {
        public:
            Pass() = default;
            explicit Pass(String name);

            const String& getName() const;
        private:
            String m_Name;
        };

        template<typename BuildFn, typename ExecFn>
        Pass& addPass(String name, BuildFn&& buildFn, ExecFn&& execFn)
        {
            Pass pass(std::move(name));
            PassBuilder builder(*this);
            buildFn(builder);

            m_Passes.emplace(std::move(pass));
            return m_Passes.last();
        }


        void execute(CommandBuffer* cmdBuffer);
    private:
        static inline uint32_t kCurrentHandle = 0;
        Array<Pass> m_Passes;
    };
}
