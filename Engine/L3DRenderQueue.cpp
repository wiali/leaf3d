/*
 * This file is part of the leaf3d project.
 *
 * Copyright 2014-2015 Emanuele Bertoldi. All rights reserved.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 *
 * You should have received a copy of the modified BSD License along with this
 * program. If not, see <http://www.opensource.org/licenses/bsd-license.php>
 */

#include <leaf3d/L3DRenderer.h>
#include <leaf3d/L3DRenderQueue.h>

using namespace l3d;

L3DRenderQueue::L3DRenderQueue(
    L3DRenderer* renderer,
    const char* name
) : L3DResource(L3D_RENDER_QUEUE, renderer),
    m_name(name)
{
    if (renderer) renderer->addRenderQueue(this);
}

L3DRenderQueue::~L3DRenderQueue()
{
    for (L3DRenderCommandList::reverse_iterator it = m_commands.rbegin(); it != m_commands.rend(); ++it)
        delete *it;
    m_commands.clear();
}

void L3DRenderQueue::appendCommands(const L3DRenderCommandList& commands)
{
    m_commands.reserve(commands.size());
    m_commands.insert(m_commands.end(), commands.begin(), commands.end());
}

void L3DRenderQueue::addSwitchFrameBufferCommand(L3DFrameBuffer* frameBuffer)
{
    m_commands.push_back(
        new L3DSwitchFrameBufferCommand(frameBuffer)
    );
}

void L3DRenderQueue::addClearBuffersCommand(
    bool colorBuffer,
    bool depthBuffer,
    bool stencilBuffer,
    const L3DVec4& clearColor
)
{
    m_commands.push_back(
        new L3DClearBuffersCommand(colorBuffer, depthBuffer, stencilBuffer, clearColor)
    );
}

void L3DRenderQueue::addSetDepthTestCommand(
    bool enable,
    const L3DDepthFactor& factor
)
{
    m_commands.push_back(
        new L3DSetDepthTestCommand(enable, factor)
    );
}

void L3DRenderQueue::addSetDepthMaskCommand(
    bool enable
)
{
    m_commands.push_back(
        new L3DSetDepthMaskCommand(enable)
    );
}

void L3DRenderQueue::addSetStencilTestCommand(
    bool enable
)
{
    m_commands.push_back(
        new L3DSetStencilTestCommand(enable)
    );
}

void L3DRenderQueue::addSetBlendCommand(
    bool enable,
    const L3DBlendFactor& srcFactor,
    const L3DBlendFactor& dstFactor
)
{
    m_commands.push_back(
        new L3DSetBlendCommand(enable, srcFactor, dstFactor)
    );
}

void L3DRenderQueue::addSetCullFaceCommand(
    bool enable,
    const L3DCullFace& cullFace
)
{
    m_commands.push_back(
        new L3DSetCullFaceCommand(enable, cullFace)
    );
}

void L3DRenderQueue::addDrawMeshesCommand(unsigned char renderLayer)
{
    m_commands.push_back(
        new L3DDrawMeshesCommand(renderLayer)
    );
}
