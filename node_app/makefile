# Deploy this Node.js project to the public folder
PROJECT_NAME = elevator-embedded-server
DEPLOY_PATH = $(HOME)/cmpt433/public/node/$(PROJECT_NAME)_copy

all: deploy npm_install

deploy:
	@echo 'COPYING THE NODE.JS FILES TO $(DEPLOY_PATH)'
	@echo ''
	mkdir -p $(DEPLOY_PATH)
	chmod a+rwx $(DEPLOY_PATH)
	cp -r * $(DEPLOY_PATH)
	@echo 'Do not edit any files in this folder; they are copied!' > $(DEPLOY_PATH)/DO_NOT_EDIT-FILES_COPIED.txt
	@echo ''
	@echo 'NOTE: On the host, in $(DEPLOY_PATH), it is best to run: npm install'
	@echo '      Or, just run the node_install target in this makefile.'

npm_install:
	@echo ''
	@echo ''
	@echo 'INSTALLING REQUIRED NODE PACKAGES'
	@echo '(This may take some time)'
	@echo ''
	cd $(DEPLOY_PATH) && npm install
	
clean:
	rm -rf $(DEPLOY_PATH)

# the make targets below should be run in the target device (not in the host)

run_server:
	@echo ''
	@echo ''
	@echo 'RUNNING DEVELOPMENT SERVER'
	@echo ''
	npm start

install_sys_dep: node_update
	
node_update:
	@echo ''
	@echo ''
	@echo 'UPDATING NODE TO v6.11.4'
	@echo '(This may take some time)'
	@echo ''
	curl -sL https://deb.nodesource.com/setup_6.x | sudo -E bash -
	sudo apt-get install -y nodejs
	