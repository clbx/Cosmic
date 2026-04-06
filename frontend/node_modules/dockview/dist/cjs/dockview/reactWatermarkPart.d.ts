import React from 'react';
import { ReactPortalStore } from '../react';
import { PanelUpdateEvent, IWatermarkRenderer, WatermarkRendererInitParameters, IWatermarkPanelProps } from 'dockview-core';
export declare class ReactWatermarkPart implements IWatermarkRenderer {
    readonly id: string;
    private readonly component;
    private readonly reactPortalStore;
    private readonly _element;
    private part?;
    private readonly parameters;
    get element(): HTMLElement;
    constructor(id: string, component: React.FunctionComponent<IWatermarkPanelProps>, reactPortalStore: ReactPortalStore);
    init(parameters: WatermarkRendererInitParameters): void;
    focus(): void;
    update(params: PanelUpdateEvent): void;
    layout(_width: number, _height: number): void;
    dispose(): void;
}
