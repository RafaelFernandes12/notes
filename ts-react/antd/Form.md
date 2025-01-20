# Form

## setFields

Form.setFields can be used to set the rules dynamically.

```tsx
<Form.Item<IAdmin> name="logo" label="Logo" rules={formRules}>
  <Upload
    onChange={(info) => {
      setFileList(info.fileList);
    }}
    beforeUpload={(file) => {
      if (!file.type.startsWith("image/")) {
        form.setFields([
          {
            name: "logo",
            errors: ["Você só pode fazer upload de arquivos de imagem!"],
          },
        ]);
        return Upload.LIST_IGNORE;
      }
      return true;
    }}
    fileList={fileList}
  >
    <PlusOutlined />
  </Upload>
</Form.Item>
```
