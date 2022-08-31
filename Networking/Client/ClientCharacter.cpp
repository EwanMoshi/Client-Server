#include "pch.h"
#include "ClientCharacter.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

ClientCharacter::ClientCharacter() {

}

void ClientCharacter::update(float deltaTime) {

}

void ClientCharacter::render() {
	std::cout << "[ClientCharacter::render]" << std::endl;
	glUseProgram(shaderProgram);

	glm::mat4 model = glm::mat4(1.0f);
	glm::mat4 view = glm::mat4(1.0f);
	glm::mat4 projection = glm::mat4(1.0f);

	//model = glm::rotate(model, glm::radians(-55.0f), glm::vec3(1.0f, 0.0f, 0.0f));
	model = glm::translate(model, glm::vec3(getX(), getY(), 0.0f));
	model = glm::scale(model, glm::vec3(50.0f, 50.0f, 0.0f));

	view = glm::translate(view, glm::vec3(0.0f, 0.0f, -3.0f));
	//projection = glm::perspective(glm::radians(45.0f), (float)800 / (float)600, 0.1f, 100.0f);
	projection = glm::ortho(0.0f, 800.0f, 0.0f, 600.0f, 0.1f, 100.0f);

	unsigned int modelLoc = glGetUniformLocation(shaderProgram, "model");
	unsigned int viewLoc = glGetUniformLocation(shaderProgram, "view");
	unsigned int projectionLoc = glGetUniformLocation(shaderProgram, "projection");

	glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
	glUniformMatrix4fv(viewLoc, 1, GL_FALSE, &view[0][0]);
	glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, glm::value_ptr(projection));

	glBindVertexArray(VAO);
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
}

void ClientCharacter::read(InputBitStream& inputStream) {
	std::cout << "[ClientCharacter::read]" << std::endl;

	uint32_t playerId;
	inputStream.read(playerId);
	setPlayerId(playerId);
	std::cout << "Read player ID " << playerId << std::endl;

	uint32_t x, y;
	inputStream.read(x);
	inputStream.read(y);
	std::cout << "Read x = " << x << " and y = " << y << std::endl;

	setLocation(x, y);
}
