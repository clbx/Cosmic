import React from 'react';
import { createGridview, PROPERTY_KEYS_GRIDVIEW, } from 'dockview-core';
import { ReactGridPanelView } from './view';
import { usePortalsLifecycle } from '../react';
function extractCoreOptions(props) {
    const coreOptions = PROPERTY_KEYS_GRIDVIEW.reduce((obj, key) => {
        if (key in props) {
            obj[key] = props[key];
        }
        return obj;
    }, {});
    return coreOptions;
}
export const GridviewReact = React.forwardRef((props, ref) => {
    const domRef = React.useRef(null);
    const gridviewRef = React.useRef();
    const [portals, addPortal] = usePortalsLifecycle();
    React.useImperativeHandle(ref, () => domRef.current, []);
    const prevProps = React.useRef({});
    React.useEffect(() => {
        const changes = {};
        PROPERTY_KEYS_GRIDVIEW.forEach((propKey) => {
            const key = propKey;
            const propValue = props[key];
            if (key in props && propValue !== prevProps.current[key]) {
                changes[key] = propValue;
            }
        });
        if (gridviewRef.current) {
            gridviewRef.current.updateOptions(changes);
        }
        else {
            // not yet fully initialized
        }
        prevProps.current = props;
    }, PROPERTY_KEYS_GRIDVIEW.map((key) => props[key]));
    React.useEffect(() => {
        if (!domRef.current) {
            return () => {
                // noop
            };
        }
        const frameworkOptions = {
            createComponent: (options) => {
                return new ReactGridPanelView(options.id, options.name, props.components[options.name], { addPortal });
            },
        };
        const api = createGridview(domRef.current, Object.assign(Object.assign({}, extractCoreOptions(props)), frameworkOptions));
        const { clientWidth, clientHeight } = domRef.current;
        api.layout(clientWidth, clientHeight);
        if (props.onReady) {
            props.onReady({ api });
        }
        gridviewRef.current = api;
        return () => {
            gridviewRef.current = undefined;
            api.dispose();
        };
    }, []);
    React.useEffect(() => {
        if (!gridviewRef.current) {
            return;
        }
        gridviewRef.current.updateOptions({
            createComponent: (options) => {
                return new ReactGridPanelView(options.id, options.name, props.components[options.name], { addPortal });
            },
        });
    }, [props.components]);
    return (React.createElement("div", { style: { height: '100%', width: '100%' }, ref: domRef }, portals));
});
GridviewReact.displayName = 'GridviewComponent';
