#include "BatchRenderer.h"

#include <glm/ext/matrix_clip_space.hpp>

#include "../../shader/ShaderUtils.h"

BatchRenderer::BatchRenderer(int maxCapacity) {
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

	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid *)offsetof(Vertex, color));

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	auto shader = new ShaderUtils(
		"../src/utils/shader/shaders/batch/vertex.glsl",
		"../src/utils/shader/shaders/batch/fragment.glsl"
	);

	mShader = shader -> ID;
}

BatchRenderer::~BatchRenderer() {
	delete[] mVertices;
}

void BatchRenderer::pushObject(Texture &texture, glm::vec3 position, glm::vec4 uv, glm::vec2 size, glm::vec4 color) {
	float rad = glm::radians(position.z);
	float cosTheta = cos(rad);
	float sinTheta = sin(rad);

	float halfWidth = (size.x / 2.0f) * 10.0f;
	float halfHeight = (size.y / 2.0f) * 10.0f;

	glm::vec2 corners[4] = {
		{halfWidth, halfHeight},
		{-halfWidth, halfHeight},
		{-halfWidth, -halfHeight},
		{halfWidth, -halfHeight}
	};

	glm::vec2 rotated[4];

	for (int i = 0; i < 4; i++) {
		rotated[i].x = position.x + (corners[i].x * cosTheta - corners[i].y * sinTheta);
		rotated[i].y = position.y + (corners[i].x * sinTheta + corners[i].y * cosTheta);
	}

	float u1 = uv.x, v1 = uv.w;
	float u2 = uv.z, v2 = uv.y;

	if (mCount + 6 >= mMaxCount) {
		flush();
	}

	pushVertex(texture, {rotated[0], {u2, v1}, color});
	pushVertex(texture, {rotated[1], {u1, v1}, color});
	pushVertex(texture, {rotated[2], {u1, v2}, color});

	pushVertex(texture, {rotated[0], {u2, v1}, color});
	pushVertex(texture, {rotated[2], {u1, v2}, color});
	pushVertex(texture, {rotated[3], {u2, v2}, color});
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
	glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(Vertex) * mCount, mVertices);

	glBindVertexArray(mVao);
	glDrawArrays(GL_TRIANGLES, 0, mCount);

	mCount = 0;
}