import ast
import astor
import sys

class ControlFlowFlattening(ast.NodeTransformer):
    def __init__(self):
        self.blocks = []
        self.state_var = ast.Name(id='state', ctx=ast.Store())
        self.state_value = 0

    def visit_FunctionDef(self, node):
        self.generic_visit(node)
        flattened_body = self.flatten_body(node.body)
        dispatcher = self.create_dispatcher(flattened_body)
        init_state = ast.Assign(targets=[self.state_var], value=ast.Constant(value=0))
        node.body = [init_state, dispatcher]
        return node

    def flatten_body(self, body):
        flattened_body = []
        for stmt in body:
            block_label = ast.Constant(value=self.state_value)
            self.state_value += 1
            self.blocks.append((block_label, stmt))
            flattened_body.append(self.create_block(block_label, stmt))
        return flattened_body

    def create_block(self, label, stmt):
        return ast.If(
            test=ast.Compare(
                left=ast.Name(id='state', ctx=ast.Load()),
                ops=[ast.Eq()],
                comparators=[label]
            ),
            body=[stmt, ast.Assign(targets=[self.state_var], value=ast.BinOp(left=label, op=ast.Add(), right=ast.Constant(value=1)))],
            orelse=[]
        )

    def create_dispatcher(self, flattened_body):
        flattened_body[-1].body.append(ast.Return(value=None))
        return ast.While(
            test=ast.Constant(value=True),
            body=flattened_body,
            orelse=[]
        )

def flatten_control_flow(source_code):
    tree = ast.parse(source_code)
    transformer = ControlFlowFlattening()
    transformed_tree = transformer.visit(tree)
    return astor.to_source(transformed_tree)

if __name__ == "__main__":
    if len(sys.argv) != 2:
        print("Usage: python flatten.py <input_file>")
        sys.exit(1)

    input_file = sys.argv[1]

    with open(input_file, 'r') as file:
        source_code = file.read()

    flattened_code = flatten_control_flow(source_code)

    output_file = "flattened_" + input_file
    with open(output_file, 'w') as file:
        file.write(flattened_code)

    print(f"Control flow flattening completed. Check '{output_file}' for the result.")
