# Apulis AI Platform deployment using Azure Resource Management (ARM) Template

You may deploy Apulis AI Platform using ARM template by clicking on the following button ("Deploy to Azure"), or using the "az" command line tool with a parameter file. To generate a parameter file, you can create a JSON file using an editor, or click on the following button, and then click "Edit parameters". You can then download a parameter file to be used by the "az" tool.

Also, you may change "Cluster Name Desired" to be the same as the resource group name if it is unique name (instead of using an arbitrarily generated cluster name).

If a cluster name is not explictly specified, the name can be retrieved from the outputs of the template deployment.

After deployment, the dev machine created will be available via SSH, and will be named "\<clustername>-dev.\<clusterlocation>.cloudapp.azure.com".

After deployment, the webportal will be available via http, and will be available at "http://\<clustername>-infra01.\<clusterlocation>.cloudapp.azure.com".

<a href="https://portal.azure.com/#create/Microsoft.Template/uri/https%3A%2F%2Fraw.githubusercontent.com%2FApulis AI Platform%2Fazure-quickstart-templates%2Fdl%2Fdlworkspace-deployment%2Fazuredeploy.json" target="_blank">
<img src="https://raw.githubusercontent.com/Azure/azure-quickstart-templates/master/1-CONTRIBUTION-GUIDE/images/deploytoazure.png"/>
</a>
<a href="http://armviz.io/#/?load=https%3A%2F%2Fraw.githubusercontent.com%2FApulis AI Platform%2Fazure-quickstart-templates%2Fdl%2Fdlworkspace-deployment%2Fazuredeploy.json" target="_blank">
<img src="https://raw.githubusercontent.com/Azure/azure-quickstart-templates/master/1-CONTRIBUTION-GUIDE/images/visualizebutton.png"/>
</a>

## Apulis AI Platform Overview

Apulis AI Platform is an open source toolkit that allows AI scientists to spin up an AI cluster in turn-key fashion. This template will spin up a Apulis AI Platform cluster in Azure. Once setup, the Apulis AI Platform cluster in Azure provides web UI and/or restful API that allows AI scientist to run job (interactive exploration, training, inferencing, data analystics) on the cluster with resource allocated by DL Workspace cluster for each job (e.g., a single node job with a couple of GPUs with GPU Direct connection, or a distributed job with multiple GPUs per node). Apulis AI Platform also provides unified job template and operating environment that allows AI scientists to easily share their job and setting among themselves and with outside community. Apulis AI Platform out-of-box supports all major deep learning toolkits (TensorFlow, CNTK, Caffe, MxNet, etc..).

For more information about the Apulis AI Platform toolkit, visit https://github.com/apulis/apulis_platform/tree/master
