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

#ifndef L3D_L3DMESH_H
#define L3D_L3DMESH_H
#pragma once

#include "leaf3d/L3DResource.h"

namespace l3d
{
    class L3DMaterial;

    class L3DMesh : public L3DResource
    {
    private:
        GLuint          m_vbo;
        GLuint          m_ebo;
        L3DMaterial*    m_material;
        VertexFormat    m_vertexFormat;
        DrawType        m_drawType;
        DrawPrimitive   m_drawPrimitive;
        unsigned int    m_vertexCount;
        unsigned int    m_indexCount;

    public:
        L3DMat4 trans;

    public:
        L3DMesh(
            float* vertices,
            unsigned int vertexCount,
            unsigned int* indices,
            unsigned int indexCount,
            L3DMaterial* material,
            const VertexFormat& VertexFormat,
            const DrawType& drawType = L3D_DRAW_STATIC,
            const DrawPrimitive& drawPrimitive = L3D_DRAW_TRIANGLES
        );
        ~L3DMesh();

        L3DMaterial*    material() const { return m_material; }
        VertexFormat    vertexFormat() const { return m_vertexFormat; }
        DrawType        drawType() const { return m_drawType; }
        DrawPrimitive   drawPrimitive() const { return m_drawPrimitive; }
        unsigned int    vertexCount() const { return m_vertexCount; }
        unsigned int    indexCount() const { return m_indexCount; }

        void translate(const L3DVec3& trans);
        void rotate(
            float radians,
            const L3DVec3& direction = glm::vec3(0.0f, 1.0f, 0.0f)
        );

    private:
        void enableVertexAttribute(
            GLuint attrib,
            GLint size,
            GLenum type,
            GLsizei stride,
            void* startPtr = 0,
            GLboolean normalized = GL_FALSE
        );
    };
}

#endif // L3D_L3DMESH_H