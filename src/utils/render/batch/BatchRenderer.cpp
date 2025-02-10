#include "BatchRenderer.h"

#include "../../shader/ShaderUtils.h"

BatchRenderer::BatchRenderer(const int maxCapacity) {
	mMaxCount = maxCapacity;
	mCount = 0;
	mVertices = new Vertex[mMaxCount];

	glGenVertexArrays(1, &mVao);
	glBindVertexArray(mVao);

	glGenBuffers(1, &mVbo);
	glBindBuffer(GL_ARRAY_BUFFER, mVbo);
	glBufferData(GL_ARRAY_BUFFER, mMaxCount * sizeof(Vertex), nullptr, GL_DYNAMIC_DRAW);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid *)offsetof(Vertex, position));

	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid *)offsetof(Vertex, uv));

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	auto shader = new ShaderUtils(
		"../src/utils/shader/shaders/sprite/vertex.glsl",
		"../src/utils/shader/shaders/sprite/fragment.glsl"
	);

	mShader = shader -> ID;
}

BatchRenderer::~BatchRenderer() {
	delete[] mVertices;
}

void BatchRenderer::pushObject(Texture &texture, glm::vec2 position, glm::vec4 uv, glm::vec2 size, glm::vec3 color) {
	float x1 = position.x;
	float y1 = position.y;
	float x2 = position.x + size.x;
	float y2 = position.y + size.y;

	float u1 = uv.x;
	float v1 = uv.y;
	float u2 = uv.z;
	float v2 = uv.w;

    if (mCount + 6 >= mMaxCount) {
        flush();
    }

    pushVertex(texture, {{x1, y1}, {u1, v1}});
    pushVertex(texture, {{x2, y2}, {u2, v2}});
    pushVertex(texture, {{x1, y2}, {u1, v2}});

    pushVertex(texture, {{x1, y1}, {u1, v1}});
    pushVertex(texture, {{x2, y1}, {u2, v1}});
    pushVertex(texture, {{x2, y2}, {u2, v2}});
}

void BatchRenderer::pushVertex(Texture &texture, Vertex vertex) {
	if (mTexture != texture.ID) {
		flush();
		mTexture = texture.ID;
	}

	if (mCount >= mMaxCount) {
		flush();
	}

	mVertices[mCount] = vertex;
	mCount++;
}

void BatchRenderer::flush() {
	if (mCount == 0) {
		return;
	}

    glUseProgram(mShader);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, mTexture);

    glUniform1i(glGetUniformLocation(mShader, "u_texture"), 0);
    glUniformMatrix4fv(glGetUniformLocation(mShader, "u_mvp"), 1, GL_FALSE, mMvp[0]);

    glBindBuffer(GL_ARRAY_BUFFER, mVbo);
    glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(Vertex) * mCount,
            mVertices);

    glBindVertexArray(mVao);
    glDrawArrays(GL_TRIANGLES, 0, mCount);

    mCount = 0;
}