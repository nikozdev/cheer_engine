{//Initialize
	{
		glGenVertexArrays(1, &VAO);
		glGenBuffers(1, &VBO);
		glBindVertexArray(VAO);
		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glEnableVertexAttribArray(0);
		if (true) {
			glGenBuffers(1, &EBO);

			glBufferData(GL_ARRAY_BUFFER, sizeof(vert_rect2d_4),
				vert_rect2d_4, GL_STATIC_DRAW);

			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
			glBufferData(GL_ELEMENT_ARRAY_BUFFER, 6 * sizeof(unsigned int),
				&indices[0], GL_STATIC_DRAW);
		}
		else {
			glBufferData(GL_ARRAY_BUFFER,
				sizeof(vert_rect2d_6), vert_rect2d_6, GL_STATIC_DRAW);
		}
		glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), 0);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindVertexArray(0);
	}
//Draw
if (true){
		if (!shaderPtr) return;
		shaderPtr->use();
		projView = camera->getViewMat();
		GLCall(shaderPtr->setMat4("projView", projView));
		if (VAO)
		{
			GLCall(glBindVertexArray(VAO));
			if (EBO)
			{
				GLCall(glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, NULL));
			}
			else
			{
				GLCall(glDrawArrays(GL_TRIANGLES, 0, 6));
			}
			glBindVertexArray(0);
		}
		shaderPtr->stopUse();
	}